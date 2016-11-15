#include "pch.h"
#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Window* window = new Window;
	if (window->Initialize())
	{
		window->Run();
	}
	delete window;
	return 0;
}
