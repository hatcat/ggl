#include "menu_platform.h"
#include "window_platform.h"
#include "application_platform.h"

ggl::menu_platform::menu_platform()
	: Menu(CreateMenu(), DestroyMenu)
{}

ggl::menu_platform::menu_platform(char* Title, const menu_platform& Parent)
	: Menu(CreatePopupMenu(), DestroyMenu)
{
	AppendMenu(Parent.menu_id(), MF_POPUP, (UINT_PTR)Menu.get(), Title);
}

ggl::menu_platform::~menu_platform()
{
//	DestroyMenu(Menu);
}

HMENU ggl::menu_platform::menu_id() const
{
	return Menu.get();
}

void ggl::menu_platform::add_leaf_impl(char* Title, command_fn Function, menu_platform& Parent)
{
	UINT Id = add_command(Function);
	MENUITEMINFO Item = {sizeof(MENUITEMINFO),	// cbSize
						MIIM_STRING | MIIM_ID,	// fMask
						0,						// fType
						0,						// fState
						Id,						// wID
						NULL,					// hSubMenu
						NULL,					// hbmpChecked
						NULL,					// hbmpUnchecked
						NULL,					// dwItemData
						Title,					// dwTypeData
						0,						// cch
						NULL};					// hbmpItem
	InsertMenuItem(Parent.menu_id(), Id, FALSE, &Item);
}

