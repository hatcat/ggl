#include "command_interpreter.h"
#include "environment_variables.h"

////////////////////////////////////////////////////////////////////////////////
// ggl::tweaker
////////////////////////////////////////////////////////////////////////////////

ggl::tweaker::tweaker(const char* Name, const char* FileName, int LineNumber, const char* Category, const char* TooltipTitle, const char* TooltipText)
	: Name(Name)
	, FileName(FileName)
	, LineNumber(LineNumber)
	, Category(Category)
	, TooltipTitle(TooltipTitle)
	, TooltipText(TooltipText)
	, Dirty(false)
{
	auto A = ggl::tweakers().insert(tweaker_collection::value_type(std::string(Name), this));
	assert(A.second);//, "(%s) already exists - have you created this tweaker in a lib rather than a dll?", Name);
}

ggl::tweaker::~tweaker()
{
	static bool Written = write_public_tweakers();
}

std::ostream& ggl::tweaker::script(std::ostream& Os) const
{
	Os << std::string(Name) << " ";
	peek(Os);
	return Os << "; # " << std::string(TooltipText) << " " << std::string(FileName) << " " << LineNumber << " #" << std::endl;
}

ggl::tweaker_collection& ggl::tweakers()
{
	static tweaker_collection TMap;
	return TMap;
}

ggl::tweaker_collection& ggl::public_tweakers()
{
	static tweaker_collection TMap;
	return TMap;
}

std::string& ggl::public_tweakers_file_name()
{
	static std::string FileName = "preferences.script";
	return FileName;
}

bool ggl::write_public_tweakers()
{
	std::ofstream File(public_tweakers_file_name());
	for (const auto& T : public_tweakers())
	{
		File << T.second;
	}
	return true;
}

std::vector<ggl::tweaker*> ggl::find_all_tweakers(const std::string& Category)
{
	std::vector<ggl::tweaker*> CategoryTweakers;
	for(const auto& T : ggl::tweakers())
	{
		if (T.second->Category == Category)
		{
			CategoryTweakers.push_back(T.second);
		}
	}
	return CategoryTweakers;
}
