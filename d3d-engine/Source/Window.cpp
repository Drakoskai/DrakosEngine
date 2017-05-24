#include "pch.h"
#include "Window.h"
#include "Utility.h"

Window::Window() :
	m_applicationName(nullptr),
	m_hinstance(nullptr),
	m_hwnd(nullptr),
	m_Application(nullptr),
	m_Input(nullptr) {}

Window::Window(const Window&) :
	m_applicationName(nullptr),
	m_hinstance(nullptr),
	m_hwnd(nullptr),
	m_Application(nullptr),
	m_Input(nullptr) {}

Window::~Window() {
	if (m_Application) {
		delete m_Application;
		m_Application = nullptr;
	}
	if (m_Input) {
		delete m_Input;
		m_Input = nullptr;
	}

	ReleaseWindow();
}

bool Window::Initialize() {
	int screenWidth = 0;
	int screenHeight = 0;
	InitWindow(screenWidth, screenHeight);
	m_Input = new InputContext;
	if (!m_Input) {
		return false;
	}

	if (!m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight)) {
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}
	ApplicationInputContext = m_Input;

	m_Application = new Game;
	if (!m_Application->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight)) {
		return false;
	}

	return true;
}

void Window::Run() const {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	bool done = false;
	while (!done) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			done = true;
		} else {
			if (!Frame()) {
				done = true;
			}
		}
	}
}

bool Window::Frame() const {
	if (m_Input->IsEscapePressed() == true) {
		return false;
	}


	if (!m_Application->Frame(m_Input)) {
		return false;
	}

	return true;
}

LRESULT CALLBACK Window::WinMsgHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	if (ApplicationInputContext) {
		ApplicationInputContext->MessageHandler(hwnd, umsg, wparam, lparam);
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void Window::InitWindow(int& screenWidth, int& screenHeight) {
	WNDCLASS wcex = { };
	int posX;
	int posY;

	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(nullptr);

	m_applicationName = L"Drakos";

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_applicationName;

	if (!RegisterClass(&wcex)) {
		MessageBox(m_hwnd, L"Could not initialize window object.", L"Error", MB_OK);
	}
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(screenWidth);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(screenHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = 0;
		posY = 0;
	} else {
		screenWidth = 1920;
		screenHeight = 1080;
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindow(m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, nullptr, nullptr, m_hinstance,
		nullptr);
	if (!m_hwnd) {

		MessageBox(m_hwnd, L"Could not create window object.", L"Error", MB_OK);

	}

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);
}

void Window::ReleaseWindow() {
	ShowCursor(true);

	if (FULL_SCREEN) {
		ChangeDisplaySettings(nullptr, 0);
	}
	DestroyWindow(m_hwnd);

	m_hwnd = nullptr;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = nullptr;

	ApplicationHandle = nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage) {
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		return ApplicationHandle->WinMsgHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
