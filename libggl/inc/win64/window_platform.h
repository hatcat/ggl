#if !defined WINDOW_PLATFORM_H
#define WINDOW_PLATFORM_H

#include "platform.h"

#include "menu_platform.h"

namespace ggl
{
	struct process_context;
	class menu_platform;

	class window_platform
	{
	public:
		static HINSTANCE register_window(HINSTANCE Instance);
		window_platform(const process_context& Context, const menu_platform& Menu);
		HWND	window_id();

	protected:
		bool	process_messages_impl();

	private:
		static LRESULT CALLBACK wnd_proc(HWND Window, UINT Message, WPARAM WordParam, LPARAM LongParam);

		HWND			Window;
		HACCEL			AcceleratorTable;
	};
}

#endif // WINDOW_PLATFORM_H