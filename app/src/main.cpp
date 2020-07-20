#if defined(WIN32)
#include <tchar.h>
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
#include <inputbox.h>
//https://cpp.hotexamples.com/examples/-/-/luaL_loadstring/cpp-lual_loadstring-function-examples.html
#include "foo_bar.h"

using namespace std;
using namespace boost;

class param {
public:
	param() = default;
	string display_name;
	string default_value;
	string description;

	string prompt() {
		return string(dependency_test::lib_a::InputBox(
			const_cast<char*>(description.c_str()), 
			const_cast<char*>(display_name.c_str()), 
			const_cast<char*>(default_value.c_str())));
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
	std::map<string, command> command_list;

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
				p.default_value = parameter_node.get<string>("default");
				p.display_name = parameter_node.get<string>("displayName");
				p.description = parameter_node.get<string>("description");
				c.param_list.push_back(p);
			}
			command_list.insert(pair<string, command>(c.name, c));
		}
	}

	void get(_TCHAR* arg) {
		string cl = command_list[arg].print();
		if (cl.empty()) {
			cout << "Invalid input" << endl;
			system("pause");
		}
		else {
			cout << cl << endl;
			system(cl.c_str());
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
	  setting s;
	  s.load("settings.json");
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