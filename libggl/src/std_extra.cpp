#include <cctype>

#include "std_extra.h"

std::string std_extra::quoted_string_from_istream(std::istream& Is)
{
	std::string Val;
	for(;;)
	{
		bool quote = false;
		char ch;
		Is.get(ch);
		if ((!quote && std::isspace(ch)) || Is.eof())
		{
			return Val;
		}
		if (ch == '\"')
		{
			if (quote)
			{
				return Val;
			}
			quote = true;
		}
		Val += ch;
	}
}