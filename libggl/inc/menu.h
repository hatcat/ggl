#if !defined MENU_H
#define MENU_H

#include "menu_platform.h"

namespace ggl
{
	class window;

	/*
	A menu is a list of titles and functions.  Some items are nodes without
	functions that host sub menus.
	*/
	class menu : public menu_platform
	{
	public:
		menu();
		menu(menu&&);
		void	add_leaf(char* Title, command_fn Function);
		menu	add_node(char* Title);

	protected:
		menu(char* Title, const menu&);

	private:
		menu(const menu&);
	};
}

#endif // MENU_H