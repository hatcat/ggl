#if !defined APPLICATION_H
#define APPLICATION_H

#include <string>

#include "application_platform.h"

namespace ggl
{
	class application : public application_platform
	{
	public:
		application(const process_context&);
		int run();
		const process_context& context(); 

	private:
		application& operator=(const application&);
		const process_context Context;
	};
}

#endif // APPLICATION_H