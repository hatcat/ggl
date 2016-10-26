#include "ggl_test.h"

#include <command_interpreter.h>
#include <application.h>
#include <window.h>

#include <sstream>

int ggl_test::main(const ggl::process_context& Context)
{
	ggl::application	App(Context);

	ggl::menu			Root;	// Create the initial menu structure

	ggl::menu			FileMenu(Root.add_node("&File"));
	FileMenu.add_leaf("E&xit",
		[](const ggl::command_context&)
		{
			ggl::commands::Quit(ggl::commands::NullStream);
		});

	ggl::menu			HelpMenu(Root.add_node("&Help"));
	HelpMenu.add_leaf("&About",
		[](const ggl::command_context&)
	{
		ggl_test::about();
	});

	ggl::window			MainWindow(Context, Root);	// Create the window

	while (!MainWindow.process_messages() && !ggl::exit_flag())
	{
		// Collect input
		// Tick the world
		// Draw the world
	}
	return 0;
}