#if defined(WIN32)
#include <tchar.h>
#include <windows.h>
#endif

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>

#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <inputbox.h>
#include <lua/lua.h>

#include "foo_bar.h"

using namespace std;
using namespace boost;

class param {
public:
	char* display_name;
	char* cli_argument;
	char* default_value;
	char* description;

	param(char* display_name, char* default_value, char* description = (char*)"") {
		this->display_name = display_name;
		this->default_value = default_value;
		this->description = description;
	}

	string prompt() {
		return string(dependency_test::lib_a::InputBox(description, display_name, default_value));
	}
};

//https://stackoverflow.com/questions/38053233/c-why-did-boost-authors-use-a-struct-here-instead-of-a-class/38053913
//MsgBox("Hello World!", 3, "MsgBox Example")
//docker run -d -p 3306:3306 -e MYSQL_ROOT_PASSWORD=password mariadb
class command {
public:
	command() = default;
	string line;
	std::vector<param> param_list;

	string print() {
		format fmter(line);
		for (param param : this->param_list) {
			fmter = fmter % param.prompt();
		}
		return fmter.str();
	}

	void load(const std::string& filename) {
		// Create empty property tree object
		boost::property_tree::ptree tree;

		// Parse the XML into the property tree.
		boost::property_tree::json_parser::read_json(filename, tree);

		this->line = tree.get<string>("command");

		//// Use the default-value version of get to find the debug level.
		//// Note that the default value is used to deduce the target type.
		//m_level = tree.get("debug.level", 0);

		//// Use get_child to find the node containing the modules, and iterate over
		//// its children. If the path cannot be resolved, get_child throws.
		//// A C++11 for-range loop would also work.

	}
};

class setting {
	std::vector<command> command_list;

	void load(const std::string& filename) {
		// Create empty property tree object
		boost::property_tree::ptree tree;

		// Parse the XML into the property tree.
		boost::property_tree::json_parser::read_json(filename, tree);

		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, tree.get_child("debug.modules")) {
			cout << v.second.data();
		}
	}
};

#if defined(WIN32)
int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
#else
int main(int /*argc*/, char* /*argv*/[])
#endif
{
  try
  {
    //dependency_test::app::foo();
    //dependency_test::app::bar();
	//docker run -d -p 3306:3306 -e MYSQL_ROOT_PASSWORD=password mariadb
	/**
	{
		"command": "docker run -d -p %1%:3306 -e MYSQL_ROOT_PASSWORD=%2% mariadb",
		"parameters": [
			{
				"default": 3306
				"display_name": "port",
				"description": ""
			}
		]
	}
	**/

	  command c;
	  c.load("settings.json");
	  c.param_list = { param("port", "3306") };
	  string cl = c.print();
	  cout << cl << endl;

	 system(cl.c_str());
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