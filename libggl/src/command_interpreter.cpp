#include "command_interpreter.h"
#include "environment_variables.h"
#include "user_feedback.h"
#include "std_extra.h"

#include <deque>
#include <algorithm>
#include <sstream>
#include <cassert>

namespace
{
	std::ostream*	CliOs = nullptr;                                         // output is initially streamed to null (i.e. not at all)
	bool			ExitFlag = false;
	std::string		CommandBuffer;

	std::unordered_map<std::string, std::ostream*>& output_streams()
	{
		static std::unordered_map<std::string, std::ostream*> Streams;
		return Streams;        
	}

	std::ostream& operator <<(std::ostream& Os, ggl::command& Command)
	{
		if (strlen(Command.TooltipTitle) == 0)	Os << "----" << std::endl;
		else									Os << Command.TooltipTitle << std::endl;
		if (strlen(Command.TooltipText) == 0)	Os << "    ----" << std::endl;
		else									Os << "    " << Command.TooltipText << std::endl;
		return Os;
	}

	template<typename T> void tweak(std::istream& is, const std::string& token, T& val)
	{
		ws(is);                                                                                         // Advance beyond the whitespace
		is >> val;
		if (!is.fail())      //CONSOLE_MSG(cli_ostream, token << " " << val << endl)
		else;                //CONSOLE_MSG(cli_ostream, token << " cannot accommodate this value." << endl);
	}
}

ggl::command::command(const char* CommandString, const char* Category, const char* TooltipTitle, const char* TooltipText, fn Command)
	: CommandString(CommandString)
	, Category(Category)
	, TooltipTitle(TooltipTitle)
	, TooltipText(TooltipText)
	, Command(Command)
{
	auto A = ggl::command_set().insert(command_collection::value_type(std::string(CommandString), this));
	assert(A.second);//, "(%s) already exists - have you created this tweaker in a lib rather than a dll?", Name);
}

std::istream& ggl::command::operator()(std::istream& Is)
{
	return Command(Is);
}

ggl::command_collection& ggl::command_set()
{
	static command_collection Cc;
	return Cc;
}

void ggl::execute(const std::string& Command)			// Execute a command held in a std::string, rather than a stream
{
	std::stringstream CommandStream;
	CommandStream << Command;
	execute(CommandStream);
}

void ggl::execute(std::istream& Is)
{
	Is >> std::boolalpha;
	while (Is.good())																	// Infinite loop, until we run out of commands
	{
		std::ws(Is);															// Advance beyond the whitespace
		if (Is.eof()) break;

		std::stringstream CommandStream;										// Command stream
		bool Comment = false;
		bool Quoted = false;
		for (;;)
		{
			char Element;
			Is.get(Element);													// Get the element
			if (!Is.good())
				break;
			else if ('#' == Element && !Quoted)
			{
				Comment = !Comment;												// Toggle comment status
				if (!Comment)
				{
					ws(Is);														// Having just closed a comment, advance beyond the whitespace
				}
			}
			else if ('\"' == Element && !Comment)
			{
				Quoted = !Quoted;												// Toggle quote status
				CommandStream.put(Element);
			}
			else if (';' == Element && !Quoted && !Comment)
				break;															// Terminate command and break
			else if (!Comment)
				CommandStream.put(Element);										// Add element to command string
		}
		std::string Token = std_extra::quoted_string_from_istream(CommandStream);	// Get the token
		if (Token.empty())
		{
			break;																// No token, empty stream, break the loop
		}
		CommandStream.copyfmt(Is);												// Copy the format flags

		auto Func = command_set().find(Token);										// Retrieve the function pointer
		if (Func != command_set().end())
		{
			ws(CommandStream);													// Advance beyond the whitespace
			(Func->second->Command)(CommandStream);								// Execute the token (don't you just love the brackets?)
			if (!CommandStream.eof())											// The command didn't parse the entire stream...
			{
				while (!CommandStream.eof() && CommandStream.good())			// ... collect what was unused
				{
					std::string tmp;
					std::ws(CommandStream);
					CommandStream >> tmp;
					//CONSOLE_MSG(cli_ostream, "tweak " << token << ": value inadmissable." << endl); // Warn of failure
				}
			}
			continue;
		}

		auto Candidate = tweakers().find(Token);									// Retrieve the function pointer
		if (Candidate != tweakers().end())
		{
			ws(CommandStream);													// Advance beyond the whitespace
			if (CommandStream.eof())											// No parameters - it's a request for a value
			{
				//CONSOLE_MSG(cli_ostream, Token << " = ");						// Print the token and an equals sign
				std::stringstream Value;
				Candidate->second->peek(Value);											// Retrieve the value
			}
			else
			{
				Candidate->second->tweak(CommandStream);												// Tweak the value
				if (CommandStream.fail())
				{
					//CONSOLE_MSG(cli_ostream, "tweak " << token << ": value inadmissable." << endl);	// Warn of failure
				}
			}
			continue;
		}

		std::ifstream CommandFile(Token);									// It's not a command or a tweak.  See if it's a file.
		if (CommandFile.is_open())
		{
			//CONSOLE_MSG(cli_ostream, "Parsing batch file " << token << ":\n");
			execute(CommandFile);												// And recurse...
			continue;
		}
	}
}

bool ggl::register_output(const std::string& name, std::ostream& stream)
{
	auto insert = output_streams().insert(std::make_pair(name, &stream));
	if (insert.second == false)
	{
		//CONSOLE_MSG(cli_ostream, name << " already exists in the output list." << endl);
		return false;
	}
	return true;
}

void ggl::redirect_output(std::ostream& Os)    // Redirect the output stream
{
	CliOs = &Os;
}

bool ggl::redirect_output(const std::string& new_stream)
{
	auto output = output_streams().find(new_stream);
	if (output == output_streams().end())
	{
		//CONSOLE_MSG(cli_ostream, new_stream << " isn't available as an output stream." << endl);
		return false;
	}
	CliOs = output->second;
	return true;
}
bool ggl::unregister_output(const std::string& name)
{
	auto output = output_streams().find(name);
	if (output == output_streams().end())
	{
		//CONSOLE_MSG(cli_ostream, name << " hasn't been registered as an output stream." << endl);
		return false;
	}
	output_streams().erase(output);
	return true;
}

std::ostream& ggl::cli_ostream()                                                // Get the output stream
{
	return *CliOs;
}

std::string& ggl::user_script_file_name()
{
	static std::string FileName = "user.script";
	return FileName;
}

bool ggl::exit_flag()
{
	return ExitFlag;
}                                         // Test the exit flag.

void ggl::buffer_command(const std::string & cmd)
{
	CommandBuffer += cmd + ';';
}

std::string ggl::buffered_commands()
{
	return CommandBuffer;
}

void ggl::clear_buffered_commands()
{
	CommandBuffer.clear();
}

////////////////////////////////////////////////////////////////////////////////
// The command library
////////////////////////////////////////////////////////////////////////////////

std::istringstream ggl::commands::NullStream;

////////////////////////////////////////////////////////////////////////////////
// preferences
////////////////////////////////////////////////////////////////////////////////

ggl::command preferences_cmd("preferences", "system", "preference \"file_name\"", "load this preference file",
							 [](std::istream& Is)->std::istream&
{
	using namespace ggl;
	std::string FileName;
	Is >> FileName;

	if (FileName.empty())
	{
		report(error_stream() << "No file name supplied\n");
	}

	std::ifstream Preferences(FileName);
	if (Preferences.good())
	{
		report(output_stream() << "Executing batch file " << FileName << std::endl);
		execute(Preferences);
	}
	else
	{
		report(error_stream() << "Failed to execute batch file " << FileName << std::endl);
	}
	return Is;
});

////////////////////////////////////////////////////////////////////////////////
// help
////////////////////////////////////////////////////////////////////////////////

ggl::command help_cmd("help", "system", "help", "list the commands",
					  [](std::istream& Is)->std::istream&
{
	using namespace ggl;
	for (auto Command : command_set())
	{
		report(output_stream() << std::string(Command.second->TooltipTitle) << std::endl << "    " << Command.second->TooltipText << std::endl << std::endl);
	}
	return Is;
});

////////////////////////////////////////////////////////////////////////////////
// docudemon
////////////////////////////////////////////////////////////////////////////////
ggl::command docudemon_cmd("docudemon", "system", "docudemon", "Create a documentation file of all CLI commands and tweaks",
						   [](std::istream& Is)->std::istream&
{
	/*
	Unleash the docudemon...
	Collect all the keyword objects from each domain.  Sort them
	alphabetically by keyword and dump them to a file.
	*/
	std::ofstream Doc("docudemon.txt");
	if (Doc.good())
	{
		// Tweakers
		Doc << "Tweakers\n" << "========\n";
		std::deque<std::string> Tweakers;
		for (auto Tweaker : ggl::tweakers())
		{
			auto& T = *(Tweaker.second);
			std::stringstream DefaultValue;
			DefaultValue << std::string(T.Category ? T.Category : "Unassigned Category") << "; "
				<< std::string(T.Name) << "; "
				<< std::string(T.TooltipText) << "; current value:"
				<< T;
			Tweakers.push_back(DefaultValue.str());
		}
		std::sort(std::begin(Tweakers), std::end(Tweakers));
		for (auto& Tweaker : Tweakers)
		{
			Doc << Tweaker << std::endl;
		}
		Doc << std::endl << std::endl << std::endl;

		// Commands
		Doc << "Command library\n";
		Doc << "===============\n";
		std::deque<std::string> Commands;
		for (auto& Command : ggl::command_set())
		{
			auto& C = *(Command.second);
			std::stringstream DefaultValue;
			DefaultValue << std::string(C.CommandString) << "; "
				<< std::string(C.Category ? C.Category : "Unassigned Category") << "; "
				<< std::string(C.TooltipTitle) << "; "
				<< std::string(C.TooltipText);
			Commands.push_back(DefaultValue.str());
		}
		std::sort(std::begin(Commands), std::end(Commands));
		for (auto& Command : Commands)
		{
			Doc << Command << std::endl;
		}
		Doc << std::endl << std::endl << std::endl;
	}
	else
	{
		ggl::report(ggl::error_stream() << "Could not open docudemon.txt\n");
	}
	return Is;
});
 
////////////////////////////////////////////////////////////////////////////////
// quit
////////////////////////////////////////////////////////////////////////////////

ggl::command ggl::commands::Quit("quit", "system", "quit", "quit the application", [](std::istream& Is)->std::istream&
{
	ExitFlag = true;															// The rest of the system will have to deal with the exit request.
	return Is;
});

////////////////////////////////////////////////////////////////////////////////
// print
////////////////////////////////////////////////////////////////////////////////
ggl::command print_cmd("print", "system", "print <text to print>", "print the subsequent text to the CLI output",
					   [](std::istream& Is)->std::istream&
{
	std::string PrintItem = std_extra::quoted_string_from_istream(Is);
	if (!PrintItem.empty())
	{
		ggl::report(ggl::output_stream() << PrintItem);
	}
	ggl::report(ggl::output_stream() << std::endl);
	return Is;
});

////////////////////////////////////////////////////////////////////////////////
// tweak
////////////////////////////////////////////////////////////////////////////////
ggl::command tweak_cmd("tweak", "system", "tweak <variable name> <new value>", "Modify the value of the named variable",
					   [](std::istream& Is)->std::istream&
{
	/*
	Find the tweaker from the first word in the stream, and then pass the
	istream to the templated tweak function.
	*/
	std::string TweakName;
	Is >> TweakName;

	if (TweakName.empty())
	{
		ggl::report(ggl::output_stream() << "tweak - nothing specified to tweak." << std::endl);
		return Is;
	}
	auto Tweaker = ggl::tweakers().find(TweakName);
	if (Tweaker == ggl::tweakers().end())
	{
		ggl::report(ggl::output_stream() << "tweak " << TweakName << ": tweaker doesn't exist." << std::endl);
		return Is;
	}
	Is >> *(Tweaker->second);
	if (Is.fail())
	{
		ggl::report(ggl::output_stream() << "tweak " << TweakName << ": value inadmissable." << std::endl);
	}
	return Is;
});

////////////////////////////////////////////////////////////////////////////////
// peek
////////////////////////////////////////////////////////////////////////////////
ggl::command peek_cmd("peek", "system", "peek <variable name>", "Display the value of the named variable",
					   [](std::istream& Is)->std::istream&
{
	/*
	Find the tweaker from the first word in the stream, and then pass the
	ostream to the templated peek function.
	*/
	std::string TweakName;
	Is >> TweakName;
	if (TweakName.empty())
	{
		ggl::report(ggl::output_stream() << "peek - nothing specified to peek." << std::endl);
		return Is;
	}
	auto Tweaker = ggl::tweakers().find(TweakName);
	if (Tweaker == ggl::tweakers().end())
	{
		ggl::report(ggl::output_stream() << "peek " << TweakName << ": tweaker doesn't exist." << std::endl);
		return Is;
	}
	ggl::report(ggl::output_stream() << TweakName << " = " << *(Tweaker->second) << std::endl);
	return Is;
});

////////////////////////////////////////////////////////////////////////////////
// flush_tweaks
////////////////////////////////////////////////////////////////////////////////
ggl::command flush_tweaks_cmd("flush_tweaks", "system", "flush_tweaks <file_name> <dirty> <category>", "Export all the tweaks in the named category to the named file, in script-executable format.  dirty should be true or false - if true, only dirty tweaks will be exported.  If the category is unspecified, all tweaks will be exported",
					   [](std::istream& Is)->std::istream&
{
	std::string FileName;
	Is >> FileName;
	if (FileName.empty())
	{
		ggl::report(ggl::output_stream() << "flush_tweaks - no file specified to write to." << std::endl);
		return Is;;
	}

	std::string DirtyId;
	Is >> DirtyId;
	bool Dirty = DirtyId == "true";
	if (!Dirty && DirtyId != "false")
	{
		ggl::report(ggl::output_stream() << "flush_tweaks - dirty flag not correctly specified - must be true or false (case sensitive)." << std::endl);
		return Is;
	}

	std::ofstream Tweaks(FileName);
	if (Tweaks.good())
	{
		std::string Category;
		Is >> Category;
		for (auto Tweaker : ggl::tweakers())
		{
			auto& T = *(Tweaker.second);
			if ((Category.empty() || T.Category == Category) && (!Dirty || T.dirty()))
			{
				T.script(Tweaks);
				T.clean();
			}
		}
	}
	else
	{
		ggl::report(ggl::output_stream() << "Could not open file " << FileName << std::endl);
	}
	return Is;
});

////////////////////////////////////////////////////////////////////////////////
// list_tweaks
////////////////////////////////////////////////////////////////////////////////
ggl::command list_tweaks_cmd("list_tweaks", "system", "list_tweaks <file_name> <category>", "Write the names of all the tweaks in the named category to the named file.  If the category is unspecified, all tweaks will be exported",
					   [](std::istream& Is)->std::istream&
{
	std::string FileName;
	Is >> FileName;
	if (FileName.empty())
	{
		ggl::report(ggl::output_stream() << "flush_tweaks - no file specified to write to." << std::endl);
		return Is;
	}

	std::ofstream Tweaks(FileName);
	if (Tweaks.good())
	{
		std::string Category;
		Is >> Category;
		for (auto Tweaker : ggl::tweakers())
		{
			auto& T = *(Tweaker.second);
			if (Category.empty() || T.Category == Category)
			{
				Tweaks << T.Name << "; # " << T.TooltipText << " " << T.FileName << " " << T.LineNumber << " #" << std::endl;
			}
		}
	}
	else
	{
		ggl::report(ggl::output_stream() << "Could not open file " << FileName << std::endl);
	}
	return Is;
});








#if 0

////////////////////////////////////////////////////////////////////////////

// change_stream

////////////////////////////////////////////////////////////////////////////

DEFINE_DEV_COMMAND(change_stream, "system", "change_stream <stream identifier>", "Redirect output to another stream, e.g. cout", is)

{

       using UTILITYLIB::endl;

       std::string new_stream;

       new_stream = unistring_from_UniCharIStream(is);

       if (new_stream.length() == 0)                                                            // Error trap

       {

              CONSOLE_MSG(cli_ostream, "change_stream: no stream provided to change to." << endl);

              return;

       }

       if (!redirect_output(new_stream))

       {

              CONSOLE_MSG(cli_ostream, "change_stream: invalid stream." << endl);        // Nope, don't recognise that keyword

       }

}

 

////////////////////////////////////////////////////////////////////////////

// console_exec

////////////////////////////////////////////////////////////////////////////

DEFINE_DEV_COMMAND(console_exec, "system", "console_exec <console command>", "forward a string into the console", is)

{

       std::string command;   

              

       while (!is.eof())                                                          // Not reached the end of the file

       {

        CA::UniChar16 nex_char = UNI_CHAR_NULL_CHARACTER;

              is.get(nex_char);          

              command += nex_char;       

       }      

#ifdef USE_WINDOW_CONSOLE

       CONSOLE::execute(command);

#endif 

}

 

////////////////////////////////////////////////////////////////////////////

// con_exec

////////////////////////////////////////////////////////////////////////////

#if defined( USE_WINDOW_CONSOLE )

 

DEFINE_DEV_COMMAND(con_exec, "system", "con_exec", "Send a command to the console rather than the game", is)

{

       WIN_CON::console_command(unistring_from_UniCharIStream(is, UNI_CHAR_NULL_CHARACTER));

}

#endif // #if defined( USE_WINDOW_CONSOLE )

 

////////////////////////////////////////////////////////////////////////////////

// rem

// Note: Wrapped into namespace to avoid a name collision when compiling with intel

////////////////////////////////////////////////////////////////////////////////

namespace ggl

{


 

       DEFINE_DEV_COMMAND(rem, "system", "rem <command>", "whatever follows this command will not execute", is)

       {      

              CA::UniChar16 nex_char = UNI_CHAR_NULL_CHARACTER;

              while (!is.eof())                                                          // Not reached the end of the file

              {             

                     is.get(nex_char);                        

              }      

       }

}

 

////////////////////////////////////////////////////////////////////////////////

// crash_test_dummy

////////////////////////////////////////////////////////////////////////////////

#pragma warning(push)

#pragma warning(disable : 4189)

void ggl::crash_test(ggl::CRASH_TEST_TYPE ctt)

{

       float32 divisor = ctt == CTT_ACCESS_VIOLATION ? 1.0f : 0.0f;

       switch (ctt)

       {

       case CTT_ACCESS_VIOLATION:

              {

                     char* p = 0;

                     p[0] = '\0';

              }

       case CTT_DIV_ZERO:

              {

                     float32 test = 1.0f / divisor;

              }

       }

}

#pragma warning(pop)

 

DEFINE_DEV_COMMAND(crash_test_dummy, "system", "crash_test_dummy <access_violation|div_zero>", "Force a crash of the named type", is)

{

       std::string type;

       is >> type;

       if (type == L"access_violation")

       {

              crash_test(CTT_ACCESS_VIOLATION);

       }

       else if (type == L"div_zero")

       {

              crash_test(CTT_DIV_ZERO);

       }

       else

       {

              CA_ERROR("Incorrect crash type specified - choose from access_violation or div_zero\n");

       }

}

 

////////////////////////////////////////////////////////////////////////////////

// set_team_asserts

////////////////////////////////////////////////////////////////////////////////

 

#if defined TEST_ASSERT_OUTPUTS

bool assert_test_flag = false;

std::string flag_string = L"flag string";

#endif

 

DEFINE_DEV_COMMAND(set_team_asserts, "system", "set_team_asserts <battle|campaign|ui|sound|multiplayer|tools|warscape|animations|art|campaign_ai|global> <true|false>", "Subscribe to or unsubscribe from team asserts", is)

{

       std::string type;

       bool flag = false;

       is >> type >> flag;

       if            (type == L"battle")               { if (flag) CA::team_subscriptions |= TI_BATTLE;                     else CA::team_subscriptions &= ~TI_BATTLE; }

       else if (type == L"campaign")            { if (flag) CA::team_subscriptions |= TI_CAMPAIGN;                   else CA::team_subscriptions &= ~TI_CAMPAIGN; }

       else if (type == L"ui")                         { if (flag) CA::team_subscriptions |= TI_UI;                         else CA::team_subscriptions &= ~TI_UI; }

       else if (type == L"sound")               { if (flag) CA::team_subscriptions |= TI_SOUND;                      else CA::team_subscriptions &= ~TI_SOUND; }

       else if (type == L"multiplayer")  { if (flag) CA::team_subscriptions |= TI_MULTIPLAYER;         else CA::team_subscriptions &= ~TI_MULTIPLAYER; }

       else if (type == L"tools")               { if (flag) CA::team_subscriptions |= TI_TOOLS;                      else CA::team_subscriptions &= ~TI_TOOLS; }

       else if (type == L"warscape")            { if (flag) CA::team_subscriptions |= TI_WARSCAPE;                   else CA::team_subscriptions &= ~TI_WARSCAPE; }

       else if (type == L"animations")          { if (flag) CA::team_subscriptions |= TI_ANIMATIONS;          else CA::team_subscriptions &= ~TI_ANIMATIONS; }

       else if (type == L"art")                 { if (flag) CA::team_subscriptions |= TI_ART;                        else CA::team_subscriptions &= ~TI_ART; }

       else if (type == L"campaign_ai")  { if (flag) CA::team_subscriptions |= TI_CAMPAIGN_AI;         else CA::team_subscriptions &= ~TI_CAMPAIGN_AI; }

       else if (type == L"campaign_design"){ if (flag) CA::team_subscriptions |= TI_CAMPAIGN_DESIGN;   else CA::team_subscriptions &= ~TI_CAMPAIGN_DESIGN; }

       else if (type == L"global")              { if (flag) CA::team_subscriptions |= TI_GLOBAL;                     else CA::team_subscriptions &= ~TI_GLOBAL; }

       else

       {

              CA_ERROR1(_U("Incorrect team %S specified: must be one of:\n\tbattle\n\tcampaign\n\tui\n\tsound\n\tmultiplayer\n\ttools\n\twarscape\nanimations\nart\ncampaign_ai\n"), type.get_temporary_uni_chars());

       }

 

#if defined TEST_ASSERT_OUTPUTS

       TEAM_ASSERT1(TI_TOOLS, "Test", assert_test_flag, L"Ping %S", flag_string);

       CA_REPORT1("Test", assert_test_flag, L"Ping %S", flag_string);

       TEAM_ERROR1(TI_TOOLS | TI_WARSCAPE, "Test", L"Ping %S", flag_string);

#endif

}

 

DEFINE_DEV_COMMAND(list_team_asserts, "System", "list_team_asserts", "View the available team asserts and subscription status",)

{

       using UTILITYLIB::endl;

       CONSOLE_MSG(cli_ostream, "Team asserts subscription status:\n"

              << "\tbattle          "    << (team_subscriptions & TI_BATTLE              ? "y" : "n") << endl

              << "\tcampaign        "    << (team_subscriptions & TI_CAMPAIGN            ? "y" : "n") << endl

              << "\tui              "    << (team_subscriptions & TI_UI                         ? "y" : "n") << endl

              << "\tsound           "    << (team_subscriptions & TI_SOUND               ? "y" : "n") << endl

              << "\tmultiplayer     "    << (team_subscriptions & TI_MULTIPLAYER         ? "y" : "n") << endl

              << "\ttools           "    << (team_subscriptions & TI_TOOLS               ? "y" : "n") << endl

              << "\twarscape        "    << (team_subscriptions & TI_WARSCAPE            ? "y" : "n") << endl

              << "\tanimations       "    << (team_subscriptions & TI_ANIMATIONS          ? "y" : "n") << endl

              << "\tart                  "       << (team_subscriptions & TI_ART                        ? "y" : "n") << endl

              << "\tcampaign_ai      "    << (team_subscriptions & TI_CAMPAIGN_AI         ? "y" : "n") << endl

              << "\tcampaign_design "    << (team_subscriptions & TI_CAMPAIGN_DESIGN     ? "y" : "n") << endl

              << "\tglobal          "    << (team_subscriptions & TI_GLOBAL              ? "y" : "n") << endl);

}

 

////////////////////////////////////////////////////////////////////////////////

// IO

////////////////////////////////////////////////////////////////////////////////

 

bool ggl::prompt()

{

//     return LUA::interpreter_is_active();

       return false;

}

 

void ggl::load_project_selector_script()

{

       std::string module_filename = must_fix_string_widen(get_module_filename());

       std::string project_selector_script_path = FileClass::create_file_name(FileClass::OWC_SCRIPT, _U("project"));

       int filename_pos = project_selector_script_path.rfind(CA::UniChar16('\\'));

       std::string local_project_selector_script_path = module_filename.substr(0, module_filename.rfind(CA::UniChar16('\\'))) + project_selector_script_path.substr(filename_pos, project_selector_script_path.length() - filename_pos);

       UniCharIFileStream project_selector_script(local_project_selector_script_path);

       if (project_selector_script.good())

       {

              project_selector_script >> cli;

       }

       else

       {

              UniCharIFileStream project_selector_script(project_selector_script_path);

              if (project_selector_script.good())

              {

                     project_selector_script >> cli;

              }

       }

}

 

void ggl::refresh_cli()

{

#if defined USE_WINDOW_CONSOLE

 

       for (COMMAND_MAP::ConstIterator c_it = commands().first(); c_it != commands().end(); ++c_it)

       {

              WIN_CON::install_command(must_fix_string_widen(c_it->key), c_it->value->m_category, c_it->value->m_tooltip_title, c_it->value->m_tooltip_text);

       }

       for (TWEAKER_MAP::ConstIterator t_it = tweakers().first(); t_it != tweakers().end(); ++t_it)

       {

              WIN_CON::install_tweak(t_it->key, t_it->value->m_category, t_it->value->m_tooltip_title, t_it->value->m_tooltip_text);

       }

#endif

}

 


 

 

// Set and clear external handler

void ggl::set_external_handler(CLI_EXTERNAL_HANDLER& handler)

{

       cli_external_handler = &handler;

}

 

void ggl::clear_external_handler()

{

       cli_external_handler = null;

}

 

#ifdef TARGETING_WIN

void * ggl::read_data_to_buffer(FilePlatform h_file, char **pBuffer, card32 &size, card32 size_to_read)

{

       DWORD  bytes_read;

       if (ReadFile(h_file, (void *)(*pBuffer), size_to_read, &bytes_read, NULL) && bytes_read == size_to_read)

       {

              *pBuffer += bytes_read;

              size += bytes_read;

              return (void *)(&(*pBuffer)[0-bytes_read]);

       }

 

       return 0;

}

std::string ggl::read_string_to_buffer(FilePlatform h_file, char **pBuffer, card32 &size)

{

       card16 len = *((card16 *)read_data_to_buffer(h_file, pBuffer, size, sizeof(card16)));

 

       read_data_to_buffer(h_file, pBuffer, size, (2*len));

 

       card16 *pBuf = &((card16 *)(*pBuffer))[-len];

       CA::UniChar16 *pUniChars = ca_new_array<CA::UniChar16>(1024);

       CA::UniChar16 *pCh = pUniChars;

 

       for (card32 i = 0; i < len; ++i)

       {

              *pCh = CA::UniChar16(*((card16 *)pBuf));

              ++pCh;

              ++pBuf;

       }

       *pCh = CA::UniChar16(0);   // Terminated

 

       std::string str(pUniChars);

 

       ca_delete_array<CA::UniChar16>(pUniChars, 1024);

 

       return str;

}

#endif 

 

// eof /////////////////////////////////////////////

 

#endif