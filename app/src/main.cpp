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
#include <boost/process.hpp>
#include <boost/process/env.hpp>
#include <boost/process/environment.hpp>
#include <boost/process/system.hpp>
#include <boost/foreach.hpp>
#include <inputbox.h>
//https://cpp.hotexamples.com/examples/-/-/luaL_loadstring/cpp-lual_loadstring-function-examples.html
#include "foo_bar.h"
#include <lua.hpp>
#include <lauxlib.h>

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

class param {
public:
	param() = default;
	string display_name;
	string default_value;
	string description;
	string evaluate;

	std::vector<Variable> variable_list;
	
	// https://www.lua.org/manual/5.1/manual.html
	int eval(string expression) {
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		luaL_dostring(L, expression.c_str());
		int temp = lua_tointeger(L, 1);
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
			int result = eval(expression);
			stringstream ss;
			ss << result;
			return ss.str();
		}
	}
};

//https://stackoverflow.com/questions/38053233/c-why-did-boost-authors-use-a-struct-here-instead-of-a-class/38053913
//MsgBox("Hello World!", 3, "MsgBox Example")
class command {
public:
	command() = default;
	string name;
	string line;  //command
	std::vector<param> param_list;

	string print() {
		format fmter(line);
		for (param param : this->param_list) {
			string input = param.prompt();
			if (input.empty()) {
				return std::string();
			}
			fmter = fmter % input;
		}

		return fmter.str();
	}
};

class setting {
	map<string, command> command_list;

public:
	void load(const std::string& filename) {
		boost::property_tree::ptree tree;

		boost::property_tree::json_parser::read_json(filename, tree, std::locale("en_US.UTF-8"));

		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, tree) {
			auto node = v.second;
			command c;
			c.name = node.get<string>("name");
			c.line = node.get<string>("command");
			c.param_list = {};

			BOOST_FOREACH(boost::property_tree::ptree::value_type &parameter, node.get_child("parameters"))
			{
				auto parameter_node = parameter.second;
				param p;
				p.variable_list = {};
				
				if (boost::optional<string> evaluate = parameter_node.get_optional<string>("evaluate"))
				{
					p.evaluate = evaluate.get();
				}
				else {
					p.default_value = parameter_node.get<string>("default");
					p.display_name = parameter_node.get<string>("displayName");
					p.description = parameter_node.get<string>("description");
				}

				if (!p.evaluate.empty()) {
					BOOST_FOREACH(boost::property_tree::ptree::value_type &variable, parameter_node.get_child("variables"))
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
				c.param_list.push_back(p);
			}
			command_list.insert(pair<string, command>(c.name, c));
		}
	}

	void get(_TCHAR* arg) {
		string cl = command_list[arg].print();
		if (cl.empty()) {
			cout << "Invalid input" << endl;
			std::system("pause");
		}
		else {
			cout << cl << endl;
			std::system(cl.c_str());
		}
	}
};

#if defined(WIN32)
int _tmain(int /*argc*/, _TCHAR* argv[])
#else
int main(int /*argc*/, char* /*argv*/[])
#endif
{
  try
  {
	  //setting s;
	  //s.load("settings.json");
	  //s.get(argv[1]);

	  auto env = boost::this_process::environment();
	  env["PATH"] = "D:\\mingw64\\bin";
	  boost::process::system("cmd", env);
	  //setenv("PATH", ", 0);


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