#if defined(WIN32)
#include <tchar.h>
#include <windows.h>
#endif

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
//#include <boost/algorithm/string.hpp>
//https://thispointer.com/find-and-replace-all-occurrences-of-a-sub-string-in-c/
#include <inputbox.h>

#include "foo_bar.h"

using namespace std;

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
	string port(dependency_test::lib_a::InputBox("port", "", "3306"));
	string env(dependency_test::lib_a::InputBox("MYSQL_ROOT_PASSWORD", "This variable is mandatory and specifies the password that will be set for the MySQL root superuser account.", "password"));

	stringstream ss;

	ss << "docker run --rm -it" << " -p " << port << ":3306" << " -e " << "MYSQL_ROOT_PASSWORD=" << env << " mariadb";

	string command = ss.str();
	cout << command << endl;

	system(command.c_str());
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