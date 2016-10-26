#include "platform.h"

#include "user_feedback.h"

#include <sstream>

void ggl::report(std::ostream& Os)
{
/*	std::stringstream msg;
	while (Os.good())
	{
		msg << Os;
	}
	OutputDebugStringA(msg.str().c_str());*/
	Os.clear();
}

std::ostream& ggl::output_stream()
{
	static std::stringstream s;
	return s;
}

std::ostream& ggl::error_stream()
{
	static std::stringstream s;
	return s;
}