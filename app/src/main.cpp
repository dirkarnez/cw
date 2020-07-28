#if defined(WIN32)
#include <tchar.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#endif

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <inputbox.h>
#include "foo_bar.h"
#include <lua.hpp>
#include <lauxlib.h>
#include <boost\filesystem\path.hpp>
#include <boost\filesystem\operations.hpp>


//#include <boost/process/environment.hpp>
#include <boost\dll\runtime_symbol_info.hpp>
using namespace std;
using namespace boost;

class Variable {
public:
	Variable() = default;
	string name;
	string display_name;
	string default_value;
	string description;
};

class Param {
public:
	Param() = default;
	string display_name;
	string default_value;
	string description;
	string evaluate;

	std::vector<Variable> variable_list;

	auto eval(string expression) {
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		luaL_dostring(L, expression.c_str());
		auto temp = lua_tointeger(L, 1);
		lua_close(L);
		return temp;
	}

	string prompt() {
		if (variable_list.empty()) {
			return string(dependency_test::lib_a::InputBox(
				const_cast<char*>(description.c_str()), 
				const_cast<char*>(display_name.c_str()), 
				const_cast<char*>(default_value.c_str())));
		}
		else {
			stringstream expression_stream;
			for (Variable variable : variable_list) {
				expression_stream << "local " << variable.name << "=" << string(dependency_test::lib_a::InputBox(
					const_cast<char*>(variable.description.c_str()),
					const_cast<char*>(variable.display_name.c_str()),
					const_cast<char*>(variable.default_value.c_str()))) << "\n";
			}
			expression_stream << "return " << evaluate;
			string expression = expression_stream.str();
			stringstream ss;
			ss << eval(expression);
			return ss.str();
		}
	}
};

class Command {
public:
	Command() = default;
	string line;
	vector<Param> param_list;

	string print() {
		format fmter(line);
		for (Param param : this->param_list) {
			string input = param.prompt();
			if (input.empty()) {
				return string();
			}
			fmter = fmter % input;
		}

		return fmter.str();
	}
};

//MsgBox("Hello World!", 3, "MsgBox Example")
// Per JSON
class WrappedCommand {
public:
	WrappedCommand() = default;
	string name;
	vector<Command> command_list;
};

class Setting {
public:
	map<string, WrappedCommand> wrapped_command_list;

	void add(const boost::filesystem::path& path) {
		using namespace boost::property_tree;

		ptree tree;
		json_parser::read_json(path.string(), tree);

		WrappedCommand wc;
		wc.name = tree.get<string>("name");
		wc.command_list = {};

		BOOST_FOREACH(boost::property_tree::ptree::value_type & command_value, tree.get_child("commands"))
		{
			auto command_node = command_value.second;
			Command command;
			command.line = command_node.get<string>("command");
	
			boost::optional<ptree&> optional_parameter_value = command_node.get_child_optional("parameters");

			if (optional_parameter_value.has_value()) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type & parameter_value, optional_parameter_value.get())
				{
					auto parameter_node = parameter_value.second;
					Param p;
					p.variable_list = {};

					if (boost::optional<string> evaluate = parameter_node.get_optional<string>("evaluate"))
					{
						p.evaluate = evaluate.get();
					}
					else {
						p.display_name = parameter_node.get<string>("displayName");
						p.description = parameter_node.get<string>("description");
						p.default_value = parameter_node.get<string>("default");
						boost::replace_all(p.default_value, "{{SCRIPT_LOCATION}}", path.branch_path().string()); 
						// buggy, see https://stackoverflow.com/questions/34466077/how-to-implement-escaping
					}

					if (!p.evaluate.empty()) {
						BOOST_FOREACH(boost::property_tree::ptree::value_type & variable, parameter_node.get_child("variables"))
						{
							auto variable_node = variable.second;
							Variable v;
							v.name = variable_node.get<string>("name");
							v.default_value = variable_node.get<string>("default");
							v.display_name = variable_node.get<string>("displayName");
							v.description = variable_node.get<string>("description");
							p.variable_list.push_back(v);
						}
					}
					command.param_list.push_back(p);
				}
			}
			
			wc.command_list.push_back(command);
		}

		wrapped_command_list.insert(pair<string, WrappedCommand>(wc.name, wc));
	}

	void get(_TCHAR* arg) {
		for (Command command : this->wrapped_command_list[arg].command_list) {
			string cl = command.print();
			if (cl.empty()) {
				cout << "Invalid input" << endl;
				std::system("pause");
			}
			else {
				cout << cl << endl;
				std::system(cl.c_str());
			}
		}
	}
};

#if defined(WIN32)
int _tmain(int /*argc*/, _TCHAR* argv[])
#else
int main(int /*argc*/, char* /*argv*/[])
#endif
{
	//_putenv(string("PATH=").append(getenv("PATH")).append("c:\\mylib;c:\\yourlib").c_str());

	//std::system("C:\\windows\\System32\\cmd.exe");

  try
  {
	  Setting s;

	  using namespace boost::filesystem;
	  
	  path p = boost::dll::program_location().parent_path() / "cw-scripts";
	  try
	  {
		  if (exists(p))
		  {
			  if (is_directory(p)) {
				  for (auto& _p : recursive_directory_iterator(p)) {
					  if (!is_directory(_p) && _p.path().extension() == ".cw") {
						  s.add(_p.path());
					  }
				  }
			  }
			  else {
				cout << p << " exists, but is not a  directory\n";
			  }
		  }
		  else {
			cout << p << " does not exist\n";
		  }
	  }
	  catch (const filesystem_error& ex)
	  {
		  cout << ex.what() << '\n';
	  }

	  s.get(argv[1]);

	  return EXIT_SUCCESS;
  }
  catch (const std::exception& e)
  {
    cerr << "Error: " << e.what() << endl;
  }
  catch (...)
  {
    cerr << "Unexpected error" << endl;
  }
  return EXIT_FAILURE;
}