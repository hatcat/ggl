/*
Environment variables are type-independent objects that can be modified at run
time.  They need unique names, regardless of type, so we have an ABC which is
type independent and a derived class template which contains the type specific
API.
*/

#if !defined ENVIRONMENT_VARIABLES_H
#define ENVIRONMENT_VARIABLES_H

#if !defined NDEBUG
#define DEBUG_VARIABLES
#endif

#include <array>
#include <string>
#include <fstream>
#include <unordered_map>
#include <cassert>

namespace ggl
{
	class tweaker
	{
	public:
								tweaker(const char* Name, const char* FileName, int LineNumber, const char* Category, const char* TooltipTitle, const char* TooltipText);
								~tweaker();

		virtual std::ostream&	peek(std::ostream&)		const	= 0;
		virtual std::istream&	tweak(std::istream&)			= 0;
		virtual std::ostream&	script(std::ostream&)	const;

		std::string				name_as_string()		const;
		void					clean();
		bool					dirty()					const;

		const char*				Name;
		const char*				FileName;
		const int				LineNumber;
		const char*				Category;
		const char*				TooltipTitle;
		const char*				TooltipText;

	protected:
		bool					Dirty;

	private:
		tweaker& operator = (const tweaker&);
	};

	template <typename T>
	class tweaker_impl : public tweaker
	{
	public:
								tweaker_impl(const char* Name, const char* FileName, int LineNumber, const char* Category, const char* TooltipTitle, const char* TooltipText, T Tweak);
		T&						operator=(const T& Rhs);
		operator				T()				const;
		T&						val();

		// tweaker
		virtual std::ostream&	peek(std::ostream& Dest)	const;
		virtual std::istream&	tweak(std::istream& Src);

	protected:
		T						Tweak;
	};
	typedef std::unordered_map<std::string, tweaker*> tweaker_collection;

	template <typename T>
	class public_tweaker : public tweaker_impl<T>
	{
	public:
		public_tweaker(const char* Name, const char* FileName, int LineNumber, const char* Category, const char* TooltipTitle, const char* TooltipText, T Tweak);
	};

	tweaker_collection&			tweakers();
	tweaker_collection&			public_tweakers();
	std::string&				public_tweakers_file_name();
	bool						write_public_tweakers();
	std::vector<tweaker*>		find_all_tweakers(const std::string& TweakerCategory);	// Find all tweakers of a particular category
	template <typename T> T		get_tweak(const std::string& Tweaker);
	template <typename T> void	set_tweak(const std::string& Tweaker, const T& Value);
}

#define DECLARE_CLI_TWEAKER(id, type) extern ggl::tweaker_impl<type> id;
#define DEFINE_CLI_TWEAKER(id, type, initial_value, category, tooltip_title, tooltip_text) ggl::tweaker_impl<type> id(#id, type(initial_value), __FILE__, __LINE__, category, tooltip_title, tooltip_text);

#define DECLARE_PUBLIC_TWEAKER(id, type) extern ggl::tweaker<type> id;
#define DEFINE_PUBLIC_TWEAKER(id, type, initial_value, category, tooltip_title, tooltip_text) ggl::public_tweaker<type> id(#id, type(initial_value), __FILE__, __LINE__, category, tooltip_title, tooltip_text);

#if !defined NDEBUG
#define DECLARE_DEBUG_TWEAKER(id, type) extern ggl::tweaker<type> id;
#define DEFINE_DEBUG_TWEAKER(id, type, initial_value, category, tooltip_title, tooltip_text) ggl::tweaker_impl<type> id(#id, type(initial_value), __FILE__, __LINE__, category, tooltip_title, tooltip_text);
#else
#define DECLARE_DEBUG_TWEAKER(id, type)
#define DEFINE_DEBUG_TWEAKER(id, type, initial_value, category, tooltip_title, tooltip_text) const type id(initial_value);
#endif

inline std::string ggl::tweaker::name_as_string() const
{
	return Name;
}

inline void ggl::tweaker::clean()
{
	Dirty = false;
}

inline bool ggl::tweaker::dirty() const
{
	return Dirty;
}

template<typename T>
inline ggl::tweaker_impl<T>::tweaker_impl(const char* Name, const char* FileName, int LineNumber, const char* Category, const char* TooltipTitle, const char* TooltipText, T Tweak)
	: ggl::tweaker(Name, FileName, LineNumber, Category, TooltipTitle, TooltipText)
	, Tweak(Tweak)
{
}

template<typename T>
inline ggl::tweaker_impl<T>::operator T() const
{
	return Tweak;
}

template<typename T>
inline T& ggl::tweaker_impl<T>::operator=(const T& Rhs)
{
	Tweak = Rhs;
	Dirty = true;
	return Tweak;
}

template<typename T>
inline std::ostream& ggl::tweaker_impl<T>::peek(std::ostream& Dest) const
{
	return Dest << Tweak;
}

template<typename T>
inline std::istream& ggl::tweaker_impl<T>::tweak(std::istream& Source)
{
	Dirty = true;
	return Source >> Tweak;
}

template<typename T>
inline T& ggl::tweaker_impl<T>::val()
{
	return Tweak;
}

template<typename T>
inline ggl::public_tweaker<T>::public_tweaker(const char* Name, const char* FileName, int LineNumber, const char* Category, const char* TooltipTitle, const char* TooltipText, T Tweak)
	: ggl::tweaker_impl(Name, FileName, LineNumber, Category, TooltipTitle, TooltipText, Tweak)
{
	auto A = ggl::public_tweakers().insert(tweaker_collection::value_type(std::string(Name), this));
	assert(A.second);//, "(%s) already exists - have you created this tweaker in a lib rather than a dll?", name);
}

template <typename T>
inline T get_tweak(const std::string& Tweaker)
{
	const tweaker* Target = tweaker::find(Tweaker);
	if (Target)
	{
		std::ostream Os;
		Target->peek(Os);
		T Value;
		Os >> Value;
		return Value;
	}
	throw;
}

template <typename T>
inline void set_tweak(const std::string& Tweaker, const T& Value)
{
	const tweaker* Target = tweaker::find(Tweaker);
	if (Target)
	{
		std::istream Is;
		Is << Value;
		Target->tweak(Is);
		return;
	}
	throw;
}

inline std::ostream& operator << (std::ostream& Os, const ggl::tweaker& Tweaker)
{
	return Tweaker.peek(Os);
}

inline std::istream& operator >> (std::istream& Is, ggl::tweaker& Tweaker)
{
	return Tweaker.tweak(Is);
}

#endif // ENVIRONMENT_VARIABLES_H