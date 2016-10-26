#if !defined HARDWARE_INPUT_H
#define HARDWARE_INPUT_H

#include "hardware_input_platform.h"
#include "../application.h"
#include "../window.h"

#undef max

#include <limits>
#include <cstdint>

namespace ggl
{
	namespace key
	{
		enum id : std::uint8_t
		{
			ESCAPE     = 0  ,
			ONE             ,
			TWO             ,
			THREE           ,
			FOUR			,
			FIVE            ,
			SIX				,
			SEVEN           ,
			EIGHT           ,
			NINE			,
			ZERO            ,
			MINUS			,
			EQUALS          ,
			BACK            ,
			TAB             ,
			Q               ,
			W               ,
			E               ,
			R               ,
			T               ,
			Y               ,
			U               ,
			I               ,
			O               ,
			P               ,
			LBRACKET        ,
			RBRACKET        ,
			RETURN          ,
			LCONTROL        ,
			A               ,
			S               ,
			D               ,
			F               ,
			G               ,
			H               ,
			J               ,
			K               ,
			L               ,
			SEMICOLON       ,
			APOSTROPHE      ,
			GRAVE           ,
			LSHIFT          ,
			BACKSLASH       ,
			Z               ,
			X               ,
			C               ,
			V               ,
			B               ,
			N               ,
			M               ,
			COMMA           ,
			PERIOD          ,
			SLASH           ,
			RSHIFT          ,
			MULTIPLY        ,
			LALT            ,
			SPACE           ,
			CAPITAL         ,
			F1              ,
			F2              ,
			F3              ,
			F4              ,
			F5              ,
			F6              ,
			F7              ,
			F8              ,
			F9              ,
			F10             ,
			NUMLOCK         ,
			SCROLL          ,
			NUMPAD7         ,
			NUMPAD8         ,
			NUMPAD9         ,
			SUBTRACT        ,
			NUMPAD4         ,
			NUMPAD5         ,
			NUMPAD6         ,
			ADD             ,
			NUMPAD1         ,
			NUMPAD2         ,
			NUMPAD3         ,
			NUMPAD0         ,
			DECIMAL         ,
			OEM_102         ,
			F11             ,
			F12             ,
			F13             ,
			F14             ,
			F15             ,
			KANA            ,
			ABNT_C1         ,
			CONVERT         ,
			NOCONVERT       ,
			YEN             ,
			ABNT_C2         ,
			NUMPADEQUALS    ,
			PREVTRACK       ,
			AT              ,
			COLON           ,
			UNDERLINE       ,
			KANJI           ,
			STOP            ,
			AX              ,
			NEXTTRACK       ,
			NUMPADENTER     ,
			RCONTROL        ,
			MUTE            ,
			CALCULATOR      ,
			PLAYPAUSE       ,
			MEDIASTOP       ,
			VOLUMEDOWN      ,
			VOLUMEUP        ,
			WEBHOME         ,
			NUMPADCOMMA     ,
			DIVIDE          ,
			SYSRQ           ,
			RALT			,
			PAUSE           ,
			HOME            ,
			UP              ,
			PRIOR           ,
			LEFT            ,
			RIGHT           ,
			END             ,
			DOWN            ,
			NEXT            ,
			INSERT          ,
			DEL				,
			LWIN            ,
			RWIN            ,
			APPS            ,
			POWER           ,
			SLEEP           ,
			WAKE            ,
			WEBSEARCH       ,
			WEBFAVORITES    ,
			WEBREFRESH      ,
			WEBSTOP         ,
			WEBFORWARD      ,
			WEBBACK         ,
			MYCOMPUTER      ,
			MAIL            ,
			MEDIASELECT		,

			NUM_KEYS		,

			ANY = NUM_KEYS	,
			UNKNOWN
		};

		struct state
		{
			bool	Up;
			bool	Down;
			bool	Held;
		};
	}

	namespace mouse
	{
		enum event : std::uint8_t
		{
			LB_UP,
			LB_DOWN,
			LB_DBL_CLICK_UP,
			LB_DBL_CLICK_DOWN,

			RB_UP,
			RB_DOWN,
			RB_DBL_CLICK_UP,
			RB_DBL_CLICK_DOWN,

			MB_UP,
			MB_DOWN,
			MB_DBL_CLICK_UP,
			MB_DBL_CLICK_DOWN,

			MOVE,
			WHEEL,

			NUM_EVENTS
		};

		struct state
		{
					state();
			float	screen_x() const;
			float	screen_y() const;

			float	X;
			float	Y;
			float	Z;
			float	LastZ;

			bool	Left;
			bool	Right;
			bool	Middle;
			bool	Control;
			bool	Shift;
		};
	}

	namespace input
	{
		void initialise(application&, window&);
		void tick();
		const mouse::state& mouse_state();
		key::state key_state(key::id);
	}
}

#endif // HARDWARE_INPUT_H
