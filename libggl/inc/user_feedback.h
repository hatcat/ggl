#if !defined USER_FEEDBACK_H
#define USER_FEEDBACK_H

#include <iostream>

namespace ggl
{
	void report(std::ostream&);

	std::ostream& output_stream();
	std::ostream& error_stream();
}

#endif // USER_FEEDBACK_H