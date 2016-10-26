#if !defined APPLICATION_PLATFORM_H
#define APPLICATION_PLATFORM_H

#include "platform.h"

namespace ggl
{
	struct process_context
	{
		process_context(HINSTANCE, HINSTANCE, LPTSTR, int);
		process_context(const process_context&) = default;

		HINSTANCE	Instance;
		HINSTANCE	PreviousInstance;
		LPTSTR		CommandLine;
		int			CommandShow;
	};
	
	struct application_platform
	{
		application_platform(const process_context&);
		application_platform& operator=(application_platform&) = delete;

		const process_context ProcessContext;
	};

	struct command_context
	{
		HWND	Window;
		LPARAM	LongParameter;

		command_context(HWND, LPARAM);
	};

	typedef void(*command_fn)(const command_context&);
	UINT add_command(command_fn);
	void execute_command(WPARAM, const command_context&);
}

#endif // APPLICATION_PLATFORM_H