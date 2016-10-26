// game_dev.cpp : Defines the entry point for the application.
//

#include "application_platform.h"
#include "window_platform.h"

#include <deque>
#include <string>

namespace
{
	std::deque<ggl::command_fn> MenuCommands;
}

ggl::process_context::process_context(HINSTANCE Instance, HINSTANCE PreviousInstance, LPTSTR CommandLine, int CommandShow)
	: Instance(Instance)
	, PreviousInstance(PreviousInstance)
	, CommandLine(CommandLine)
	, CommandShow(CommandShow)
{}

ggl::application_platform::application_platform(const ggl::process_context& Context)
	: ProcessContext(Context)
{
	window_platform::register_window(Context.Instance);
}

ggl::command_context::command_context(HWND Window, LPARAM LongParameter)
	: Window(Window)
	, LongParameter(LongParameter)
{}

UINT ggl::add_command(command_fn Command)
{
	MenuCommands.push_back(Command);
	return (UINT)(MenuCommands.size() - 1);
}

void ggl::execute_command(WPARAM Id, const command_context& Context)
{
	MenuCommands[Id](Context);
}
