#include "application_platform.h"
#include "../ggl_test.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	return ggl_test::main(ggl::process_context(hInstance, hPrevInstance, lpCmdLine, nCmdShow));
}

void ggl_test::about()
{
	MessageBox(NULL, "Version 0.1", "About ggl_test", MB_OK);
}