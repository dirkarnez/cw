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

#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/node.h>








#include <algorithm>
#include <random>
#include <elements.hpp>

using namespace cycfi::elements;

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

auto make_controls(view& view_)
{
	auto  check_box1 = check_box("Reionizing electrons");
	auto  check_box2 = check_box("The Nexus Meridian Unfolding");
	auto  check_box3 = check_box("Serenity Dreamscape Exploration");

	check_box1.value(true);
	check_box2.value(true);
	check_box3.value(true);

	auto  check_boxes =
		group("Check boxes",
			margin({ 10, 10, 20, 20 },
				top_margin(25,
					vtile(
						top_margin(10, align_left(check_box1)),
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

	auto  icon_buttons =
		group("Icon Buttons",
			margin({ 10, 10, 20, 10 },
				vtile(
					top_margin(35,
						htile(
							align_center(toggle_icon_button(icons::power, 1.2, indicator_color)),
							align_center(icon_button(icons::magnifying_glass, 1.2)),
							align_center(icon_button(icons::left_circled, 1.2)),
							align_center(toggle_icon_button(icons::left, icons::right, 1.2))
						)
					)
				)
			)
		);

	float const button_scale = 1.0 / 4;
	sprite power_button = sprite{ "power_180x632.png", 158 * button_scale, button_scale };
	sprite phase_button = sprite{ "phase_180x632.png", 158 * button_scale, button_scale };
	sprite mail_button = sprite{ "mail_180x632.png", 158 * button_scale, button_scale };
	sprite transpo_button = sprite{ "transpo_180x632.png", 158 * button_scale, button_scale };

	auto  sprite_buttons =
		group("Sprite Buttons",
			margin({ 10, 10, 20, 10 },
				vtile(
					top_margin(35,
						htile(
							align_center(toggle_button(power_button)),
							align_center(toggle_button(phase_button)),
							align_center(momentary_button(mail_button)),
							align_center(toggle_button(transpo_button))
						)
					)
				)
			)
		);

	return
		vtile(
			htile(
				make_buttons(view_),
				vtile(
					margin({ 20, 20, 20, 20 }, check_boxes),
					margin({ 20, 20, 20, 20 }, radio_buttons)
				)
			),
			htile(
				hmin_size(250, margin({ 20, 20, 20, 20 }, icon_buttons)),
				hmin_size(250, margin({ 20, 20, 20, 20 }, sprite_buttons))
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
		//for (Command command : this->wrapped_command_list[arg].command_list) {
		//	string cl = command.print();
		//	if (cl.empty()) {
		//		cout << "Invalid input" << endl;
		//		std::system("pause");
		//	}
		//	else {
		//		cout << cl << endl;
		//		std::system(cl.c_str());
		//	}
		//}
	}
};

//#if defined(WIN32)
//int _tmain(int argc, _TCHAR* argv[])
//#else
//int main(int argc, char* argv[])
//#endif
int main(int argc, char* argv[])
{
	//_putenv(string("PATH=").append(getenv("PATH")).append("c:\\mylib;c:\\yourlib").c_str());

	//std::system("C:\\windows\\System32\\cmd.exe");


	//YAML::Node config = YAML::LoadFile("../../../config.yaml");

	//if (config["name"]) {
	//	cout << "name: " << config["name"].as<string>() << "\n";
	//}

	app _app(argc, argv, "Buttons", "com.cycfi.buttons");
	window _win(_app.name());
	_win.on_close = [&_app]() { _app.stop(); };

	view view_(_win);

	view_.content(
		make_controls(view_),
		background
	);

	_app.run();

  //try
  //{
	 // Setting s;

	 // using namespace boost::filesystem;
	 // 
	 // path p = boost::dll::program_location().parent_path() / "cw-scripts";
	 // try
	 // {
		//  if (exists(p))
		//  {
		//	  if (is_directory(p)) {
		//		  for (auto& _p : recursive_directory_iterator(p)) {
		//			  if (!is_directory(_p) && _p.path().extension() == ".cw") {
		//				  s.add(_p.path());
		//			  }
		//		  }
		//	  }
		//	  else {
		//		cout << p << " exists, but is not a directory\n";
		//	  }
		//  }
		//  else {
		//	cout << p << " does not exist\n";
		//  }
	 // }
	 // catch (const filesystem_error& ex)
	 // {
		//  cout << ex.what() << '\n';
	 // }

	 // s.get(argv[1]);

	 // return EXIT_SUCCESS;
  //}
  //catch (const std::exception& e)
  //{
  //  cerr << "Error: " << e.what() << endl;
  //}
  //catch (...)
  //{
  //  cerr << "Unexpected error" << endl;
  //}
  //return EXIT_FAILURE;
}