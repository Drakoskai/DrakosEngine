#include "pch.h"
#include "Program.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Program* program = new Program;
	if (program->Initialize())
	{
		program->Run();
	}
	delete program;
	return 0;
}
