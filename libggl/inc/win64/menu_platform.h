#if !defined MENU_PLATFORM_H
#define MENU_PLATFORM_H

#include "platform.h"
#include "application_platform.h"

#include <memory>

namespace ggl
{
	class window_platform;

	typedef std::unique_ptr<std::remove_pointer<HMENU>::type, decltype(&DestroyMenu)> raii_menu;

	class menu_platform
	{
	public:
		static void execute(UINT, window_platform&);

		menu_platform();
		menu_platform(menu_platform&&);
		~menu_platform();

		HMENU menu_id() const;

	protected:
		menu_platform(char* Title, const menu_platform& Parent);
		void add_leaf_impl(char* Title, command_fn Function, menu_platform& Parent);

	private:
		menu_platform(const menu_platform&);
		raii_menu Menu;
	};
}

#endif // MENU_PLATFORM_H