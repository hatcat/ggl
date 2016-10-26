#include "application.h"
#include "environment_variables.h"
#include "command_interpreter.h"

#include <fstream>

ggl::application::application(const process_context& Context)
	: application_platform(Context)
	, Context(Context)
{
	// Parse the preferences
	std::ifstream preferences(public_tweakers_file_name());
	execute(preferences);

	// Parse the user script
	std::ifstream user_script(user_script_file_name());
	execute(user_script);

	// Parse the command line
	execute(Context.CommandLine);
}

int ggl::application::run()
{
	return 0;
}
