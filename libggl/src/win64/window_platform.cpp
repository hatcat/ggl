#include "window_platform.h"
#include "application_platform.h"
#include "menu_platform.h"

#define IDC_GAME_DEV			109
#define IDI_GAME_DEV			107
#define IDI_SMALL				108

LRESULT CALLBACK ggl::window_platform::wnd_proc(HWND Window, UINT Message, WPARAM WordParam, LPARAM LongParam)
{
	PAINTSTRUCT PaintStruct;
	HDC DeviceContext;

	switch (Message)
	{
	case WM_PAINT:
		DeviceContext = BeginPaint(Window, &PaintStruct);
		// TODO: Add any drawing code here...
		EndPaint(Window, &PaintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		execute_command(WordParam, command_context(Window, LongParam));
		break;
	default:
		return DefWindowProc(Window, Message, WordParam, LongParam);
	}
	return 0;
}

HINSTANCE ggl::window_platform::register_window(HINSTANCE Instance)
{
	// Register the window class
	WNDCLASSEX Wcex;
	Wcex.cbSize			= sizeof(WNDCLASSEX);
	Wcex.style			= CS_HREDRAW | CS_VREDRAW;
	Wcex.lpfnWndProc	= wnd_proc;
	Wcex.cbClsExtra		= 0;
	Wcex.cbWndExtra		= 0;
	Wcex.hInstance		= Instance;
	Wcex.hIcon			= LoadIcon(Instance, MAKEINTRESOURCE(IDI_GAME_DEV));
	Wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	Wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	Wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GAME_DEV);
	Wcex.lpszClassName	= _U("ggl");
	Wcex.hIconSm		= LoadIcon(Wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&Wcex);

	return Instance;
}

ggl::window_platform::window_platform(const process_context& Context, const menu_platform& Menu)
{
	// Create the window
	Window = CreateWindow(_U("ggl"), _U("Welcome to ggl"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		NULL, NULL, Context.Instance, NULL);

	if (!Window)
	{
		throw;
	}

	ShowWindow(Window, Context.CommandShow);
	UpdateWindow(Window);

	// Load the accelerators
	AcceleratorTable = LoadAccelerators(Context.Instance, MAKEINTRESOURCE(IDC_GAME_DEV));

	SetMenu(Window, Menu.menu_id());
	DrawMenuBar(Window);
}

HWND ggl::window_platform::window_id()
{
	return Window;
}

bool ggl::window_platform::process_messages_impl()
{
	// Empty the message queue
	MSG Message;
	while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);            
		if(Message.message == WM_QUIT)
		{
			return true;
		}
	}
	return false;
}