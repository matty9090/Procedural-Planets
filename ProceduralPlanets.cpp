#include "Window.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Window app(hInstance, nCmdShow);
	app.setDims(1280, 960);

	app.init();
	app.run();

	return 0;
}