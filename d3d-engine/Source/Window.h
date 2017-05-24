#pragma once

#include <windows.h>
#include "Game.h"

class Window {
public:
	Window();
	~Window();

	bool Initialize();
	void Run() const;
	static LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	Window(const Window&);

	bool Frame() const;
	void InitWindow(int& screenWidth, int& screenHeight);
	void ReleaseWindow();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	Game* m_Application;
	InputContext* m_Input;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
static InputContext* ApplicationInputContext = nullptr;
static Window* ApplicationHandle = nullptr;
