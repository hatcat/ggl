#include <dinput.h>

#include <cassert>

#include "../hardware_input.h"

namespace
{
	using namespace ggl;

	struct direct_input_mapping
	{
		key::id			Key;
		int				DirectInputKey;
		const char *	Name;
	};

	const direct_input_mapping DIMappings[key::NUM_KEYS] =
	{
		{ key::ESCAPE,			DIK_ESCAPE,			"ESCAPE" },
		{ key::ONE,				DIK_1,				"1" },
		{ key::TWO,				DIK_2,				"2" },
		{ key::THREE,			DIK_3,				"3" },
		{ key::FOUR,			DIK_4,				"4" },
		{ key::FIVE,			DIK_5,				"5" },
		{ key::SIX,				DIK_6,				"6" },
		{ key::SEVEN,			DIK_7,				"7" },
		{ key::EIGHT,			DIK_8,				"8" },
		{ key::NINE,			DIK_9,				"9" },
		{ key::ZERO,			DIK_0,				"0" },
		{ key::MINUS,			DIK_MINUS,			"MINUS" },
		{ key::EQUALS,			DIK_EQUALS,			"EQUALS" },
		{ key::BACK,			DIK_BACK,			"BACK" },
		{ key::TAB,				DIK_TAB,			"TAB" },
		{ key::Q,				DIK_Q,				"Q" },
		{ key::W,				DIK_W,				"W" },
		{ key::E,				DIK_E,				"E" },
		{ key::R,				DIK_R,				"R" },
		{ key::T,				DIK_T,				"T" },
		{ key::Y,				DIK_Y,				"Y" },
		{ key::U,				DIK_U,				"U" },
		{ key::I,				DIK_I,				"I" },
		{ key::O,				DIK_O,				"O" },
		{ key::P,				DIK_P,				"P" },
		{ key::LBRACKET,		DIK_LBRACKET,		"LBRACKET" },
		{ key::RBRACKET,		DIK_RBRACKET,		"RBRACKET" },
		{ key::RETURN,			DIK_RETURN,			"RETURN" },
		{ key::LCONTROL,		DIK_LCONTROL,		"LCONTROL" },
		{ key::A,				DIK_A,				"A" },
		{ key::S,				DIK_S,				"S" },
		{ key::D,				DIK_D,				"D" },
		{ key::F,				DIK_F,				"F" },
		{ key::G,				DIK_G,				"G" },
		{ key::H,				DIK_H,				"H" },
		{ key::J,				DIK_J,				"J" },
		{ key::K,				DIK_K,				"K" },
		{ key::L,				DIK_L,				"L" },
		{ key::SEMICOLON,		DIK_SEMICOLON,		"SEMICOLON" },
		{ key::APOSTROPHE,		DIK_APOSTROPHE,		"APOSTROPHE" },
		{ key::GRAVE,			DIK_GRAVE,			"GRAVE" },
		{ key::LSHIFT,			DIK_LSHIFT,			"LSHIFT" },
		{ key::BACKSLASH,		DIK_BACKSLASH,		"BACKSLASH" },
		{ key::Z,				DIK_Z,				"Z" },
		{ key::X,				DIK_X,				"X" },
		{ key::C,				DIK_C,				"C" },
		{ key::V,				DIK_V,				"V" },
		{ key::B,				DIK_B,				"B" },
		{ key::N,				DIK_N,				"N" },
		{ key::M,				DIK_M,				"M" },
		{ key::COMMA,			DIK_COMMA,			"COMMA" },
		{ key::PERIOD,			DIK_PERIOD,			"PERIOD" },
		{ key::SLASH,			DIK_SLASH,			"SLASH" },
		{ key::RSHIFT,			DIK_RSHIFT,			"RSHIFT" },
		{ key::MULTIPLY,		DIK_MULTIPLY,		"MULTIPLY" },
		{ key::LALT,			DIK_LMENU,			"LALT" },
		{ key::SPACE,			DIK_SPACE,			"SPACE" },
		{ key::CAPITAL,			DIK_CAPITAL,		"CAPITAL" },
		{ key::F1,				DIK_F1,				"F1" },
		{ key::F2,				DIK_F2,				"F2" },
		{ key::F3,				DIK_F3,				"F3" },
		{ key::F4,				DIK_F4,				"F4" },
		{ key::F5,				DIK_F5,				"F5" },
		{ key::F6,				DIK_F6,				"F6" },
		{ key::F7,				DIK_F7,				"F7" },
		{ key::F8,				DIK_F8,				"F8" },
		{ key::F9,				DIK_F9,				"F9" },
		{ key::F10,				DIK_F10,			"F10" },
		{ key::NUMLOCK,			DIK_NUMLOCK,		"NUMLOCK" },
		{ key::SCROLL,			DIK_SCROLL,			"SCROLL" },
		{ key::NUMPAD7,			DIK_NUMPAD7,		"NUMPAD7" },
		{ key::NUMPAD8,			DIK_NUMPAD8,		"NUMPAD8" },
		{ key::NUMPAD9,			DIK_NUMPAD9,		"NUMPAD9" },
		{ key::SUBTRACT,		DIK_SUBTRACT,		"SUBTRACT" },
		{ key::NUMPAD4,			DIK_NUMPAD4,		"NUMPAD4" },
		{ key::NUMPAD5,			DIK_NUMPAD5,		"NUMPAD5" },
		{ key::NUMPAD6,			DIK_NUMPAD6,		"NUMPAD6" },
		{ key::ADD,				DIK_ADD,			"ADD" },
		{ key::NUMPAD1,			DIK_NUMPAD1,		"NUMPAD1" },
		{ key::NUMPAD2,			DIK_NUMPAD2,		"NUMPAD2" },
		{ key::NUMPAD3,			DIK_NUMPAD3,		"NUMPAD3" },
		{ key::NUMPAD0,			DIK_NUMPAD0,		"NUMPAD0" },
		{ key::DECIMAL,			DIK_DECIMAL,		"DECIMAL" },
		{ key::OEM_102,			DIK_OEM_102,		"OEM_102" },
		{ key::F11,				DIK_F11,			"F11" },
		{ key::F12,				DIK_F12,			"F12" },
		{ key::F13,				DIK_F13,			"F13" },
		{ key::F14,				DIK_F14,			"F14" },
		{ key::F15,				DIK_F15,			"F15" },
		{ key::KANA,			DIK_KANA,			"KANA" },
		{ key::ABNT_C1,			DIK_ABNT_C1,		"ABNT_C1" },
		{ key::CONVERT,			DIK_CONVERT,		"CONVERT" },
		{ key::NOCONVERT,		DIK_NOCONVERT,		"NOCONVERT" },
		{ key::YEN,				DIK_YEN,			"YEN" },
		{ key::ABNT_C2,			DIK_ABNT_C2,		"ABNT_C2" },
		{ key::NUMPADEQUALS,	DIK_NUMPADEQUALS,	"NUMPADEQUALS" },
		{ key::PREVTRACK,		DIK_PREVTRACK,		"PREVTRACK" },
		{ key::AT,				DIK_AT,				"AT" },
		{ key::COLON,			DIK_COLON,			"COLON" },
		{ key::UNDERLINE,		DIK_UNDERLINE,		"UNDERLINE" },
		{ key::KANJI,			DIK_KANJI,			"KANJI" },
		{ key::STOP,			DIK_STOP,			"STOP" },
		{ key::AX,				DIK_AX,				"AX" },
		{ key::NEXTTRACK,		DIK_NEXTTRACK,		"NEXTTRACK" },
		{ key::NUMPADENTER,		DIK_NUMPADENTER,	"NUMPADENTER" },
		{ key::RCONTROL,		DIK_RCONTROL,		"RCONTROL" },
		{ key::MUTE,			DIK_MUTE,			"MUTE" },
		{ key::CALCULATOR,		DIK_CALCULATOR,		"CALCULATOR" },
		{ key::PLAYPAUSE,		DIK_PLAYPAUSE,		"PLAYPAUSE" },
		{ key::MEDIASTOP,		DIK_MEDIASTOP,		"MEDIASTOP" },
		{ key::VOLUMEDOWN,		DIK_VOLUMEDOWN,		"VOLUMEDOWN" },
		{ key::VOLUMEUP,		DIK_VOLUMEUP,		"VOLUMEUP" },
		{ key::WEBHOME,			DIK_WEBHOME,		"WEBHOME" },
		{ key::NUMPADCOMMA,		DIK_NUMPADCOMMA,	"NUMPADCOMMA" },
		{ key::DIVIDE,			DIK_DIVIDE,			"DIVIDE" },
		{ key::SYSRQ,			DIK_SYSRQ,			"SYSRQ" },
		{ key::RALT,			DIK_RMENU,			"RALT" },
		{ key::PAUSE,			DIK_PAUSE,			"PAUSE" },
		{ key::HOME,			DIK_HOME,			"HOME" },
		{ key::UP,				DIK_UP,				"UP" },
		{ key::PRIOR,			DIK_PRIOR,			"PAGE_UP" },
		{ key::LEFT,			DIK_LEFT,			"LEFT" },
		{ key::RIGHT,			DIK_RIGHT,			"RIGHT" },
		{ key::END,				DIK_END,			"END" },
		{ key::DOWN,			DIK_DOWN,			"DOWN" },
		{ key::NEXT,			DIK_NEXT,			"PAGE_DOWN" },
		{ key::INSERT,			DIK_INSERT,			"INSERT" },
		{ key::DEL,				DIK_DELETE,			"DELETE" },
		{ key::LWIN,			DIK_LWIN,			"LWIN" },
		{ key::RWIN,			DIK_RWIN,			"RWIN" },
		{ key::APPS,			DIK_APPS,			"APPS" },
		{ key::POWER,			DIK_POWER,			"POWER" },
		{ key::SLEEP,			DIK_SLEEP,			"SLEEP" },
		{ key::WAKE,			DIK_WAKE,			"WAKE" },
		{ key::WEBSEARCH,		DIK_WEBSEARCH,		"WEBSEARCH" },
		{ key::WEBFAVORITES,	DIK_WEBFAVORITES,	"WEBFAVORITES" },
		{ key::WEBREFRESH,		DIK_WEBREFRESH,		"WEBREFRESH" },
		{ key::WEBSTOP,			DIK_WEBSTOP,		"WEBSTOP" },
		{ key::WEBFORWARD,		DIK_WEBFORWARD,		"WEBFORWARD" },
		{ key::WEBBACK,			DIK_WEBBACK,		"WEBBACK" },
		{ key::MYCOMPUTER,		DIK_MYCOMPUTER,		"MYCOMPUTER" },
		{ key::MAIL,			DIK_MAIL,			"MAIL" },
		{ key::MEDIASELECT,		DIK_MEDIASELECT,	"MEDIASELECT" }
	};

	HWND					WindowHandle = nullptr;
	IDirectInput8 *			DI8 = nullptr;
	IDirectInputDevice8 *	DIKeyboard = nullptr;
	IDirectInputDevice8 *	DIMouse = nullptr;
	BYTE					DIKeys[256] = { 0 };
	BYTE					PreviousKeys[256] = { 0 };
	DIMOUSESTATE			DIMouseState;
	bool					MouseCooperativeLevelIsExclusive = false;

	bool					PreviousLeft2ClickDown = false;
	bool					PreviousRight2ClickDown = false;
	bool					PreviousMiddle2ClickDown = false;

	const char* MouseEventLabels[mouse::NUM_EVENTS] = {
		"LBUP",
		"LBDOWN",
		"LB2CLICKUP",
		"LB2CLICKDOWN",
		"RBUP",
		"RBDOWN",
		"RB2CLICKUP",
		"RB2CLICKDOWN",
		"MBNUP",
		"MBDOWN",
		"MB2CLICKUP",
		"MB2CLICKDOWN",
		"MOVE",
		"WHEEL"
	};

	mouse::state	MouseState;

	bool set_DI_mouse_cooperative_level(HWND window_handle, bool exclusive, bool reacquire)
	{
		if (reacquire == true)
		{
			DIMouse->Unacquire();
		}

		DWORD flags = DISCL_FOREGROUND;

		if (exclusive == true)
		{
			flags |= DISCL_EXCLUSIVE;
		}
		else
		{
			flags |= DISCL_NONEXCLUSIVE;
		}

		if (DIMouse->SetCooperativeLevel(window_handle, flags) != DI_OK)
		{
			return false;
		}

		MouseCooperativeLevelIsExclusive = exclusive;

		if (reacquire == true)
		{
			if (DIMouse->Acquire() != DI_OK)
			{
				return false;
			}
		}

		return true;
	}

	bool acquire_DI_mouse(window& window)
	{
		if (DI8->CreateDevice(GUID_SysMouse, &DIMouse, NULL) == DI_OK)
		{
			if (DIMouse->SetDataFormat(&c_dfDIMouse) == DI_OK)
			{
				if (set_DI_mouse_cooperative_level(window.window_id(), false, false) == true)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool read_DI_keyboard(void)
	{
		memcpy(&(PreviousKeys[0]), &(DIKeys[0]), sizeof(BYTE) * 256);

		const HRESULT hr = DIKeyboard->GetDeviceState(256, DIKeys);

		switch (hr)
		{
		case DIERR_INPUTLOST:
			if (DI_OK == DIKeyboard->Acquire())
			{
				if (DI_OK == DIKeyboard->GetDeviceState(256, DIKeys))
				{
					return true;
				}

				DIKeyboard->Unacquire();
			}
			return false;

		case DIERR_NOTACQUIRED:
			if (DI_OK == DIKeyboard->Acquire())
			{
				if (DI_OK == DIKeyboard->GetDeviceState(256, DIKeys))
				{
					return true;
				}
				DIKeyboard->Unacquire();
			}
			return false;
		}

		return (hr == DI_OK);
	}

	void update_DI_mouse_state(void)
	{
		DIMouseState.direct_x_axis_delta	= float(MouseState.X);
		DIMouseState.direct_y_axis_delta	= -float(MouseState.Y);
		DIMouseState.direct_z_axis_delta	= -float(MouseState.Z);
		DIMouseState.direct_left			= (MouseState.rgbButtons[0] & 128) ? true : false;
		DIMouseState.direct_middle			= (MouseState.rgbButtons[2] & 128) ? true : false;
		DIMouseState.direct_right			= (MouseState.rgbButtons[1] & 128) ? true : false;
	}

	void clear_DI_mouse_state(void)
	{
		DIMouseState.direct_x_axis_delta	= 0.0f;
		DIMouseState.direct_y_axis_delta	= 0.0f;
		DIMouseState.direct_z_axis_delta	= 0.0f;
		DIMouseState.direct_left			= false;
		DIMouseState.direct_middle			= false;
		DIMouseState.direct_right			= false;
	}

	bool read_DI_mouse(void)
	{
		memset(&MouseState, 0, sizeof(MouseState));

		const HRESULT hr = DIMouse->GetDeviceState(sizeof(MouseState), &MouseState);

		switch (hr)
		{
		case DIERR_INPUTLOST:
			if (DI_OK == DIMouse->Acquire())
			{
				if (DI_OK == DIMouse->GetDeviceState(sizeof(MouseState), &MouseState))
				{
					return true;
				}

				DIMouse->Unacquire();
			}
			return false;

		case DIERR_NOTACQUIRED:
			if (DI_OK == DIMouse->Acquire())
			{
				if (DI_OK == DIMouse->GetDeviceState(sizeof(MouseState), &MouseState))
				{
					return true;
				}
				DIMouse->Unacquire();
			}
			return false;
		}

		return (hr == DI_OK);
	}

	bool set_mouse_cooperative_level(bool exclusive)
	{
		return set_DI_mouse_cooperative_level(WindowHandle, exclusive, true);
	}

	bool mouse_cooperative_level_is_exclusive(void)
	{
		return MouseCooperativeLevelIsExclusive;
	}

	key::id encode_scan_code(LPARAM LongParam)
	{
		using namespace key;

		const WPARAM WordParam = HIWORD(LongParam) & 0x1ff;

		switch (WordParam)
		{
		case  43: return OEM_102;
		case  69: return PAUSE;
		case  86: return BACKSLASH;
		case  87: return id(84);
		case  88: return id(85);

		case 284: return NUMPADENTER;
		case 285: return id(105);
		case 306: return WEBHOME;
		case 309: return DIVIDE;
		case 311: return SYSRQ;
		case 312: return id(116);
		case 325: return NUMLOCK;
		case 327: return HOME;
		case 328: return id(119);
		case 329: return PRIOR;
		case 331: return id(121);
		case 333: return id(122);
		case 335: return END;
		case 336: return id(124);
		case 337: return NEXT;
		case 338: return INSERT;
		case 339: return DEL;

		case 347: return LWIN; // WIN left
		case 348: return RWIN; // WIN right
		case 349: return RWIN; // WIN menu

		case 357: return WEBSEARCH;
		case 364: return MAIL;

		default: return id(WordParam - 1);
		}
	}

	float	mouse_wheel_cyclical_difference(float from, float to)
	{
		const float cycle_max = float(0xffff);

		float positive_delta = 0.0f;
		float negative_delta = 0.0f;

		if (from > to)
		{
			positive_delta = (cycle_max - from) + to; 
			negative_delta = to - from;
		}
		else if (to > from)
		{
			positive_delta = to - from;
			negative_delta = -((cycle_max - to) + from);
		}

		float delta = 0.0f;

		if (positive_delta < -negative_delta)
		{
			delta = positive_delta;
		}
		else
		{
			delta = negative_delta;
		}

		// invert for game purposes

		return -delta;
	}

/*	HARDWARE_CONTROLLER_STATE :: HARDWARE_CONTROLLER_STATE(void) :
		m_mouse_state(DIMouseState),
		m_gesture_details(n_gesture_state)
	{
	}

	HARDWARE_CONTROLLER_STATE :: HARDWARE_CONTROLLER_STATE(const MOUSE_STATE &mouse_state) :
		m_mouse_state(mouse_state)
	{
		DIMouseState_valid = true;
		n_gesture_state.event_type = GESTURE_EVENT_NONE;
	}

	HARDWARE_CONTROLLER_STATE :: HARDWARE_CONTROLLER_STATE(const GestureEventDetails& details) :
		m_gesture_details(details)
	{
		DIMouseState_valid = false;
	}

	HARDWARE_CONTROLLER_STATE :: ~HARDWARE_CONTROLLER_STATE(void)
	{
	}*/

	key::id key_from_name(const char * name)
	{
		using namespace key;

		for (auto n = 0; n<NUM_KEYS; ++n)
		{
			if (strcmp(DIMappings[n].Name, name) == 0)
			{
				return DIMappings[n].Key;
			}
		}
		return UNKNOWN;
	}

	const char * name_from_key(key::id kt)
	{
		using namespace key;

		if (kt < NUM_KEYS)
		{
			return DIMappings[kt].Name;
		}
		else
		{
			// Unassigned key
			return "";
		}
	}

	const char * name_from_mouse_event(mouse::event mt)
	{
		using namespace mouse;
		if (mt < NUM_EVENTS)
		{
			return MouseEventLabels[mt];
		}
		else
		{
			// Unassigned key
			return "";
		}
	}


	std::wstring localised_key_name(key::id key)
	{
		using namespace key;
		// Allowing unassigned keys now so assert no longer valid for KT_unknown
		assert(key <= UNKNOWN && key != NUM_KEYS);

		if (key >= NUM_KEYS)
		{
			// Unassigned key
			return L"";
		}

		// work-a-round DI turning off the num-lock when you ask for a key name!
		BYTE key_states[256];
		BOOL ok = GetKeyboardState(key_states);
#ifdef _DEBUG
		if (!ok)
		{
			// 			CA_ERROR2("GetKeyboardState() failed with error %d : %s",CA::GetLastError(), CA_PC::GetLastErrorDescription());
		}
#endif // _DEBUG

		DIPROPSTRING key_name;
		key_name.diph.dwSize       = sizeof(key_name);
		key_name.diph.dwHeaderSize = sizeof(key_name.diph);
		key_name.diph.dwObj        = DIMappings[key].DirectInputKey;
		key_name.diph.dwHow        = DIPH_BYOFFSET;//DIPH_BYID; 
		if (DIKeyboard->GetProperty(DIPROP_KEYNAME, &key_name.diph))
		{
			if (ok)
			{
				SetKeyboardState(key_states);
			}

			return L"";
		}

		if (ok)
		{
			SetKeyboardState(key_states);
		}

		return std::wstring(&key_name.wsz[0]);
	}

/*	bool HARDWARE_CONTROLLER_STATE :: is_key_held(KEY_TYPE key) const
	{
		if (GetForegroundWindow() == n_window_handle)
		{
			return ((DIKeys[DIMappings[key].direct_input_key] & 0x80) >  0);// &&
			//((n_prev_keys[DIMappings[key].direct_input_key] & 0x80) >  0);
		}

		return false;
	}

	bool HARDWARE_CONTROLLER_STATE :: is_key_down(KEY_TYPE key) const
	{
		if (GetForegroundWindow() == n_window_handle)
		{
			return ((DIKeys[DIMappings[key].direct_input_key] & 0x80) >  0) &&
				((n_prev_keys[DIMappings[key].direct_input_key] & 0x80) == 0);
		}

		return false;
	}

	bool HARDWARE_CONTROLLER_STATE :: is_key_up(KEY_TYPE key) const
	{
		if (GetForegroundWindow() == n_window_handle)
		{
			return ((DIKeys[DIMappings[key].direct_input_key] & 0x80) == 0) &&
				((n_prev_keys[DIMappings[key].direct_input_key] & 0x80) >  0);
		}

		return false;
	}

	const MOUSE_STATE* HARDWARE_CONTROLLER_STATE :: mouse_state(void) const
	{
		return DIMouseState_valid ? &m_mouse_state : NULL;
	}

	const CA::GestureEventDetails* HARDWARE_CONTROLLER_STATE::gesture_details() const
	{
		return m_gesture_details.event_type != GESTURE_EVENT_NONE && m_gesture_details.event_type != GESTURE_EVENT_END ? &m_gesture_details : NULL;
	}*/

}

key::event_details ggl::key::information(const hardware_event& ev)
{
	using namespace key;

	switch (ev.Message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		return event_details{ KEY_DOWN, encode_scan_code(ev.LongParam) };
	case WM_KEYUP:
	case WM_SYSKEYUP:
		return event_details{ KEY_UP, encode_scan_code(ev.LongParam) };
	}

	return event_details{ NONE, NUM_KEYS };
}

mouse::event ggl::mouse::information(const hardware_event& ev, std::uint32_t client_width, std::uint32_t client_height, std::uint32_t client_y_offset)
{
	using namespace mouse;

	DIMouseState_valid = true;
	n_gesture_state.event_type = GESTURE_EVENT_NONE;

	const std::uint32_t key_flags = ev.WordParam;									// key flags 
	const std::uint16_t client_x = LOWORD(ev.LongParam);							// horizontal position of cursor 
	const std::uint16_t client_y = std::uint16_t(HIWORD(ev.LongParam) + client_y_offset);	// vertical position of cursor 

	float mouse_x = float(client_x) / client_width;
	float mouse_y = float(client_y) / client_height;

	// it's inside, so let's process

	client_z = std::int16_t(client_z + HIWORD(ev.WordParam));	// mousewheel

	if (ev.message != WM_MOUSELEAVE && n_tracking_mouseleave == false)
	{
		TRACKMOUSEEVENT tm = { sizeof (TRACKMOUSEEVENT), TME_LEAVE, ev.window_handle, 0 };
		TrackMouseEvent(&tm);
		DIMouseState.m_tracking_mouseleave = true;
	}

	DIMouseState.left = (key_flags & MK_LBUTTON) ? true : false;
	DIMouseState.right = (key_flags & MK_RBUTTON) ? true : false;
	DIMouseState.middle = (key_flags & MK_MBUTTON) ? true : false;
	DIMouseState.control = (key_flags & MK_CONTROL) ? true : false;
	DIMouseState.shift = (key_flags & MK_SHIFT) ? true : false;
	DIMouseState.from_touch = from_touch;

	// Bugfix: 
	// Coordinates that are bundled with WM_MOUSEWHEEL are in screen space, rather than client space.
	// Correct coordinates are supplied with WM_MOUSEMOVE every time the cursor position changes any way, so just ignore them here.

	if (ev.message == WM_MOUSEWHEEL)
	{
		DIMouseState.last_z = DIMouseState.z;
		DIMouseState.z = float(client_z);
	}
	else
	{
		if (ev.message == WM_MOUSELEAVE)
		{
			// WM_MOUSELEAVE passes in 0,0 every time, so we ignore that, and since we're leaving
			// the client rect we make sure the mouse coords are clamped appropriately before we do

			mouse_x = (DIMouseState.x > 0.95f) ? 0.95f : ((DIMouseState.x < 0.05f) ? 0.05f : DIMouseState.x);
			mouse_y = (DIMouseState.y > 0.95f) ? 0.95f : ((DIMouseState.y < 0.05f) ? 0.05f : DIMouseState.y);
		}

		DIMouseState.x = mouse_x;
		DIMouseState.y = mouse_y;
	}

	switch (ev.message)
	{
	case WM_LBUTTONUP:
	{
						 if (n_previous_left_dbl_click_down)
						 {
							 n_previous_left_dbl_click_down = false;
							 return LBUTTONDBLCLICKUP;
						 }
						 else
						 {
							 return LBUTTONUP;
						 }
	}
	case WM_LBUTTONDOWN:
	{
						   return LBUTTONDOWN;
	}
	case WM_LBUTTONDBLCLK:
	{
							 n_previous_left_dbl_click_down = true;
							 return LBUTTONDBLCLICKDOWN;
	}
	case WM_RBUTTONUP:
	{
						 if (n_previous_right_dbl_click_down)
						 {
							 n_previous_right_dbl_click_down = false;
							 return RBUTTONDBLCLICKUP;
						 }
						 else
						 {
							 return RBUTTONUP;
						 }
	}
	case WM_RBUTTONDOWN:
	{
						   return RBUTTONDOWN;
	}
	case WM_RBUTTONDBLCLK:
	{
							 n_previous_right_dbl_click_down = true;
							 return RBUTTONDBLCLICKDOWN;
	}
	case WM_MBUTTONUP:
	{
						 if (n_previous_middle_dbl_click_down)
						 {
							 n_previous_middle_dbl_click_down = false;
							 return MBUTTONDBLCLICKUP;
						 }
						 else
						 {
							 return MBUTTONUP;
						 }
	}
	case WM_MBUTTONDOWN:
	{
						   return MBUTTONDOWN;
	}
	case WM_MBUTTONDBLCLK:
	{
							 n_previous_middle_dbl_click_down = true;
							 return MBUTTONDBLCLICKDOWN;
	}
	case WM_MOUSEMOVE:
	{
						 return MOUSEMOVE;
	}
	case WM_MOUSEWHEEL:
	{
						  return MOUSEWHEEL;
	}
	case WM_MOUSELEAVE:
	{
						  DIMouseState.m_tracking_mouseleave = false;	// The mouseleave tracker get's cancelled when it is fired, so make sure we reinstate it later
						  return MOUSELEAVE;
	}
	}

	return NONE;
}

bool ggl::acquire_hardware_controllers(application& app, window& window)
{
	const HRESULT hr = DirectInput8Create(app.ProcessContext.Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&DI8), nullptr);

	if (hr != DI_OK)
	{
		return false;
	}

	if (acquire_DI_keyboard(window_handle) == false)
	{
		return false;
	}

	if (acquire_DI_mouse(window_handle) == false)
	{
		return false;
	}

	n_window_handle = window_handle.get_hwnd();

	return true;
}

bool ggl::read_hardware_controllers(void)
{
	clear_DI_mouse_state();

	if (read_DI_keyboard() == false)
	{
		return false;
	}

	if (read_DI_mouse() == false)
	{
		return false;
	}

	update_DI_mouse_state();

	return true;
}

void ggl::release_hardware_controllers(void)
{
	if (DIKeyboard != null)
	{
		DIKeyboard->Release();
		DIKeyboard = null;
	}

	if (DIMouse != null)
	{
		DIMouse->Release();
		DIMouse = null;
	}

	if (n_di != null)
	{
		n_di->Release();
		n_di = null;
	}

	n_window_handle = null;
}



// eof /////////////////////////////////////////////

