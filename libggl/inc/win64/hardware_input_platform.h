#if !defined HARDWARE_INPUT_PLATFORM_H
#define HARDWARE_INPUT_PLATFORM_H

#include <windows.h>

namespace ggl
{
	struct hardware_event
	{
		HWND	Window;
		UINT	Message;
		WPARAM	WordParam;
		LPARAM	LongParam;
	};
}

#endif // HARDWARE_INPUT_PLATFORM_H