#pragma once

#include <windows.h>
#include <dinput.h>
#include "Mouse.h"
#include "InputListener.h"

class InputContext {
public:
	InputContext();
	~InputContext();

	bool Initialize(HINSTANCE, HWND, int, int);
	bool MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool IsEscapePressed() const;
	void GetMouseLocation(int&, int&) const;
	void GetKeyEvent(KeyEvent& e) const;
	bool IsLeftPressed() const;
	bool IsRightPressed() const;
	bool IsUpPressed() const;
	bool IsDownPressed() const;
	bool IsAPressed() const;
	bool IsDPressed() const;
	bool IsSPressed() const;
	bool IsWPressed() const;
	bool IsZPressed() const;
	bool IsPgUpPressed() const;
	bool IsPgDownPressed() const;
	bool IsF1Toggled();
	bool IsF2Toggled();
	bool IsF3Toggled();
	bool IsF4Toggled();
	bool IsF5Toggled();

private:
	InputContext(const InputContext& other);

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_keyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_mouse;

	unsigned char m_keyboardState[256];
	Mouse mMouse;
	DIMOUSESTATE m_mouseState;

	int m_screenWidth;
	int m_screenHeight;

	bool m_F1_released;
	bool m_F2_released;
	bool m_F3_released;
	bool m_F4_released;
	bool m_F5_released;

};
