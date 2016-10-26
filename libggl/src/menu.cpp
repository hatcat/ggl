#include "menu.h"
#include "window.h"

ggl::menu::menu()
{
}

ggl::menu::menu(char* Title, const menu& Parent)
	: menu_platform(Title, Parent)
{
}

void ggl::menu::add_leaf(char* Title, command_fn Function)
{
	add_leaf_impl(Title, Function, *this);
}

ggl::menu ggl::menu::add_node(char* Title)
{
	return menu(Title, *this); 
}
