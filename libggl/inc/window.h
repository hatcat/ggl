#if !defined WINDOW_H
#define WINDOW_H

#include "menu.h"
#include "window_platform.h"

namespace ggl
{
	struct process_context;

	class window : public window_platform
	{
	public:
		window(const process_context&, const menu&);
		bool process_messages();

	private:
		window& operator=(const window&);
		const menu&	Menu;
	};
}

#endif // WINDOW_H