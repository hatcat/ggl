/*
Commands are functions that can be executed from a command line interface.  The
constructor takes a lambda as a parameter.
*/

#if !defined COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

#if !defined NDEBUG
#define DEBUG_COMMANDS
#endif

#include <string>
#include <istream>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace ggl
{
	struct command
	{
		typedef std::istream& (*fn)(std::istream&);
		command(const char* CommandString, const char* Category, const char* TooltipTitle, const char* TooltipText, fn Command);
		std::istream& operator()(std::istream&);

		const char*		CommandString;
		const char*		Category;
		const char*		TooltipTitle;
		const char*		TooltipText;
		fn				Command;
	};
	typedef std::unordered_map<std::string, command*> command_collection;

	command_collection&			command_set();														// These are the commands mapped against the function pointers
	void						execute(const std::string& Command);								// Execute a specific command (good for windows GUI app lpCmdLine)
	void						execute(std::istream& Is);											// Execute commands pulled from a stream

	bool						register_output(const std::string& StreamId, std::ostream& Os);		// Register a stream, with an identifying string, as a suitable place to redirect console output
	bool						redirect_output(const std::string& StreamId);						// By default, output is sent to cout.  Use this function to redirect it to a previously registered ostream
	void						redirect_output(std::ostream& Os);									// By default, output is sent to cout.  Use this function to redirect it to a previously registered ostream
	bool						unregister_output(const std::string& StreamId);						// Mark a stream as no longer valid for output
	std::ostream&				cli_ostream();														// Retrieve the current output stream

	std::string&				user_script_file_name();
	bool						exit_flag();														// Test for the exit command
	void						buffer_command(const std::string & Command);
	std::string					buffered_commands();
	void						clear_buffered_commands();

namespace commands
{
	extern std::istringstream NullStream;
	extern command Quit;
}

}

#endif // COMMAND_INTERPRETER_H