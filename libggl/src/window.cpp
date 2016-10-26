#include "window.h"

#include <memory>

ggl::window::window(const process_context& Context, const menu& Menu)
	: window_platform(Context, Menu)
	, Menu(Menu)
{}

bool ggl::window::process_messages()
{
	return process_messages_impl();
}