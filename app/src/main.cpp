#if defined(WIN32)
#include <tchar.h>
#include <windows.h>
#endif

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>

#include "boost/format.hpp"
#include <inputbox.h>

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

	  command c{};
	  c.line = "docker run --rm -it -p %1%:3306 -e MYSQL_ROOT_PASSWORD=1234 mariadb";
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