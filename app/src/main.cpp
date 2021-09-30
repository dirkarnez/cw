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
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

//#include <boost/process/environment.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/node.h>


#include <algorithm>
#include <random>
#include <elements.hpp>

using namespace cycfi::elements;
using namespace boost::filesystem;

// Main window background color
auto constexpr bkd_color = rgba(35, 35, 37, 255);
auto background = box(bkd_color);

constexpr auto bred = colors::red.opacity(0.4);
constexpr auto bgreen = colors::green.level(0.7).opacity(0.4);
constexpr auto bblue = colors::blue.opacity(0.4);
constexpr auto brblue = colors::royal_blue.opacity(0.4);
constexpr auto pgold = colors::gold.opacity(0.8);

auto make_buttons(view& view_)
{
	auto mbutton = button("Momentary Button");
	auto tbutton = toggle_button("Toggle Button", 1.0, bred);
	auto lbutton = share(latching_button("Latching Button", 1.0, bgreen));
	auto reset = button("Clear Latch", icons::lock_open, 1.0, bblue);
	auto note = button(icons::cog, "Setup", 1.0, brblue);
	auto prog_bar = share(progress_bar(rbox(colors::black), rbox(pgold)));
	auto prog_advance = button("Advance Progress Bar");

	reset.on_click =
		[lbutton, &view_](bool) mutable
	{
		lbutton->value(0);
		view_.refresh(*lbutton);
	};

	prog_advance.on_click =
		[prog_bar, &view_](bool) mutable
	{
		auto val = prog_bar->value();
		if (val > 0.9)
			prog_bar->value(0.0);
		else
			prog_bar->value(val + 0.125);
		view_.refresh(*prog_bar);
	};

	return
		margin({ 20, 0, 20, 20 },
			vtile(
				top_margin(20, mbutton),
				top_margin(20, tbutton),
				top_margin(20, hold(lbutton)),
				top_margin(20, reset),
				top_margin(20, note),
				top_margin(20, vsize(25, hold(prog_bar))),
				top_margin(20, prog_advance)
			)
		);
}



template<typename T>
auto make_controls(view& view_, std::vector<T>& command_list)
{
	std::vector<std::shared_ptr<static_text_box>> a;

	auto  check_box1 = share(check_box("Reionizing electrons"));
	auto  check_box2 = check_box("The Nexus Meridian Unfolding");
	auto  check_box3 = check_box("Serenity Dreamscape Exploration");

	check_box1->value(true);
	check_box2.value(true);
	check_box3.value(true);

	auto  check_boxes =
		group("Check boxes",
			margin({ 10, 10, 20, 20 },
				top_margin(25,
					vtile(
						top_margin(10, align_left(hold(check_box1))),
						top_margin(10, align_left(check_box2)),
						top_margin(10, align_left(check_box3))
					)
				)
			)
		);

	auto  radio_button1 = radio_button("Eons from now");
	auto  radio_button2 = radio_button("Ultra-sentient particles");
	auto  radio_button3 = radio_button("The stratosphere is electrified");

	radio_button1.select(true);

	auto  radio_buttons =
		group("Radio Buttons",
			margin({ 10, 10, 20, 20 },
				top_margin(25,
					vtile(
						top_margin(10, align_left(radio_button1)),
						top_margin(10, align_left(radio_button2)),
						top_margin(10, align_left(radio_button3))
					)
				)
			)
		);

	auto indicator_color = get_theme().indicator_color;

	vtile_composite container;

	/*for (Command command : command_list) {*/
		//format fmter(command.line);
			//string input = param.prompt();
			// param.

			//return string(dependency_test::lib_a::InputBox(
			//	const_cast<char*>(description.c_str()),
			//	const_cast<char*>(display_name.c_str()),
			//	const_cast<char*>(default_value.c_str())));

			//if (input.empty()) {
			//	cout << "Invalid input" << endl;
			//	std::system("pause");
			//}
			//fmter = fmter % input;


	/*template <typename Container, typename Base>
//auto make_controls(view& view_, std::function<composite<Container, Base>(std::function<string(void)>)> renderer)
*/

		//string cl = fmter.str();
		//if (cl.empty()) {
		//	cout << "Invalid input" << endl;
		//	std::system("pause");
		//}
		//else {
		//	cout << cl << endl;
		//	std::system(cl.c_str());
		//}
	Command cmd = command_list.at(0);

	for (Param p : cmd.param_list) {
		auto textbox = share(static_text_box(p.description));
		auto input_pair = input_box("Show me more");

		a.push_back(input_pair.second);

		container.push_back(share(group(p.display_name,
			margin({ 10, 10, 20, 20 },
				top_margin(25,
					vtile(
						top_margin(10, left_caption(input_pair.first, p.display_name)),
						//label(p.description)
						hmin_size(350, top_margin(10, hold(textbox)))
					)
				)
			)
		)));
	}

	auto executeButton = button("Momentary Button");
	executeButton.on_click = [a, cmd](bool) mutable {
		string line = cmd.line;

		format fmter(line);

		for (auto reader : a) {
			fmter = fmter % reader->get_text();
		}

		auto output = fmter.str();

		MessageBox(
			NULL,
			/*(LPCWSTR)(L"sdf")*/
			(LPCWSTR)(std::wstring(output.begin(), output.end()).c_str()),
			(LPCWSTR)(L"sdf"),
			MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
		);

		//std::system(cl.c_str());
	};

	auto  icon_buttons =
		vtile(
			top_margin(35,
				vtile(
					top_margin(20, executeButton)
				)
			)
		);

	return
		vtile(
			vscroller(margin({ 20, 20, 20, 20 }, container)),
			htile(
				hmin_size(250, margin({ 20, 20, 20, 20 }, icon_buttons))
				//, hmin_size(250, margin({ 20, 20, 20, 20 }, sprite_buttons))
			)
		);
}


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

	//string prompt() {
	//	if (variable_list.empty()) {
	//		return string(dependency_test::lib_a::InputBox(
	//			const_cast<char*>(description.c_str()), 
	//			const_cast<char*>(display_name.c_str()), 
	//			const_cast<char*>(default_value.c_str())));
	//	}
	//	else {
	//		stringstream expression_stream;
	//		for (Variable variable : variable_list) {
	//			expression_stream << "local " << variable.name << "=" << string(dependency_test::lib_a::InputBox(
	//				const_cast<char*>(variable.description.c_str()),
	//				const_cast<char*>(variable.display_name.c_str()),
	//				const_cast<char*>(variable.default_value.c_str()))) << "\n";
	//		}
	//		expression_stream << "return " << evaluate;
	//		string expression = expression_stream.str();
	//		stringstream ss;
	//		ss << eval(expression);
	//		return ss.str();
	//	}
	//}
};

class Command {
public:
	string line;
	vector<Param> param_list;
};

// Per JSON
class WrappedCommand {
public:
	string name;
	vector<Command> command_list;
};

class Setting {
public:
	Setting(path p) {
		try
		{
			if (!exists(p))
			{
				cout << p << " does not exist\n";
				return;
			}

			if (!is_directory(p)) {
				cout << p << " exists, but is not a directory\n";
				return;
			}

			for (auto& _p : recursive_directory_iterator(p)) {
				if (!is_directory(_p) && _p.path().extension() == ".yml") {
					add(_p.path());
				}
			}
		}
		catch (const filesystem_error& ex)
		{
			cout << ex.what() << '\n';
		}
	}

	vector<Command> get(char* arg) {
		return this->wrapped_command_list[arg].command_list;
	}
private:
	map<string, WrappedCommand> wrapped_command_list;

	void add(path p) {
		using namespace YAML;

		cout << "LoadFile: " << p.string() << endl;

		Node fileNode = LoadFile(p.string());

		WrappedCommand wrappedCommand;

		Node nameNode = fileNode["name"];
		if (nameNode) {
			wrappedCommand.name = nameNode.as<string>();
			cout << "name: " << wrappedCommand.name << endl;
		}

		Node commandsNode = fileNode["commands"];

		if (commandsNode) {
			wrappedCommand.command_list = {};

			vector<Node> commands = commandsNode.as<vector<Node>>();

			for (int i = 0; i < commands.size(); i++) {
				Node commandNode = commands.at(i);

				Node commandCommandNode = commandNode["command"];
				Command command;
				
				if (commandCommandNode) {
					command.line = commandCommandNode.as<string>();
					cout << "command: " << command.line << endl;
				}

				vector<Node> parameters = commandNode["parameters"].as<vector<Node>>();
			
				for (int j = 0; j < parameters.size(); j++) {
					Node parameterNode = parameters.at(j);
					Param p;


					p.default_value = parameterNode["default"].as<string>();
					p.display_name = parameterNode["displayName"].as<string>();
					p.description = parameterNode["description"].as<string>();

					cout << "default: " << p.default_value << endl;
					cout << "displayName: " << p.display_name << endl;
					cout << "description: " << p.description << endl;

					command.param_list.push_back(p);
				}
				wrappedCommand.command_list.push_back(command);
			
			}
		}

		wrapped_command_list.insert(pair<string, WrappedCommand>(wrappedCommand.name, wrappedCommand));
	}
};
//
//template <typename T, std::size_t... I>
//auto read_row(std::vector<T> data, std::index_sequence<I...>)
//{
//	return vtile(data.at(I)...);
//}
//
//template <std::size_t N, typename T>
//auto read_row(std::vector<T> data)
//{
//	return read_row(data, std::make_index_sequence<N>{});
//}

//template <int data>
//struct MultiplicationLine<0, target> {
//	const int value = 0;
//	vtile(rest);
//};


//#if defined(WIN32)
//int _tmain(int argc, _TCHAR* argv[])
//#else
//int main(int argc, char* argv[])
//#endif
int main(int argc, char* argv[])
{
	//_putenv(string("PATH=").append(getenv("PATH")).append("c:\\mylib;c:\\yourlib").c_str());

	Setting s(boost::dll::program_location().parent_path() / "cw-scripts");

	  try
	  {
			char* name = argv[1];

			if (name == nullptr) {

				//return EXIT_SUCCESS;
				name = "mssql";
			}

			app _app(argc, argv, "Buttons", "com.cycfi.buttons");
			window _win(_app.name());
			_win.on_close = [&_app]() { _app.stop(); };

			view view_(_win);

			view_.content(make_controls(view_, s.get(name)), background);

			_app.run();

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