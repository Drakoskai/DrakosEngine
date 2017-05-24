#include "pch.h"
#include "InputContext.h"

InputContext::InputContext() :
	m_directInput(nullptr),
	m_keyboard(nullptr),
	m_mouse(nullptr),
	m_screenWidth(0),
	m_screenHeight(0),
	m_F1_released(false),
	m_F2_released(false),
	m_F3_released(false),
	m_F4_released(false),
	m_F5_released(false) {}

InputContext::InputContext(const InputContext&) :
	m_directInput(nullptr),
	m_keyboard(nullptr),
	m_mouse(nullptr),
	m_screenWidth(0),
	m_screenHeight(0),
	m_F1_released(false),
	m_F2_released(false),
	m_F3_released(false),
	m_F4_released(false),
	m_F5_released(false) {}

InputContext::~InputContext() {}

bool InputContext::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight) {
	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	mMouse = Mouse();
	// Initialize the location of the mouse on the screen.
	// Initialize the main direct input interface.
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(m_directInput.GetAddressOf()), nullptr);
	if (FAILED(result)) {
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, m_keyboard.GetAddressOf(), nullptr);
	if (FAILED(result)) {
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) {
		return false;
	}

	// Set the cooperative level of the keyboard to share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) {
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result)) {
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, m_mouse.GetAddressOf(), nullptr);
	if (FAILED(result)) {
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) {
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) {
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result)) {
		return false;
	}

	m_F1_released = true;
	m_F2_released = true;
	m_F3_released = true;
	m_F4_released = true;
	m_F5_released = true;

	return true;
}

bool InputContext::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (!ReadKeyboard()) {
		return false;
	}

	if (!ReadMouse()) {
		return false;
	}

	ProcessInput();

	return true;
}

bool InputContext::ReadKeyboard() {
	// Read the keyboard device.
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), static_cast<LPVOID>(&m_keyboardState));
	if (FAILED(result)) {
		// If the keyboard lost focus or was not acquired then try to get control back.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			m_keyboard->Acquire();
		} else {
			return false;
		}
	}

	return true;
}

bool InputContext::ReadMouse() {
	// Read the mouse device.
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), static_cast<LPVOID>(&m_mouseState));
	if (FAILED(result)) {
		// If the mouse lost focus or was not acquired then try to get control back.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			m_mouse->Acquire();
		} else {
			return false;
		}
	}

	return true;
}

void InputContext::GetKeyEvent(KeyEvent& e) const {
	e.aKey = m_keyboardState[DIK_A] & 0x80;
	e.dKey = m_keyboardState[DIK_D] & 0x80;
	e.sKey = m_keyboardState[DIK_S] & 0x80;
	e.wKey = m_keyboardState[DIK_W] & 0x80;
}

void InputContext::ProcessInput() {
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	mMouse.x += m_mouseState.lX;
	mMouse.y += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (mMouse.x < 0) { mMouse.x = 0; }
	if (mMouse.y < 0) { mMouse.y = 0; }

	if (mMouse.x > m_screenWidth) { mMouse.x = m_screenWidth; }
	if (mMouse.y > m_screenHeight) { mMouse.y = m_screenHeight; }
}

bool InputContext::IsEscapePressed() const {
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80) {
		return true;
	}

	return false;
}

void InputContext::GetMouseLocation(int& mouseX, int& mouseY) const {
	mouseX = mMouse.x;
	mouseY = mMouse.y;
}

bool InputContext::IsLeftPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_LEFT] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsRightPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_RIGHT] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsUpPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_UP] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsDownPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_DOWN] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsAPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_A] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsDPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_D] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsSPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_S] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsWPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_W] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsZPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_Z] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsPgUpPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_PGUP] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsPgDownPressed() const {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_PGDN] & 0x80) {
		return true;
	}

	return false;
}

bool InputContext::IsF1Toggled() {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_F1] & 0x80) {
		if (m_F1_released) {
			m_F1_released = false;
			return true;
		}
	} else {
		m_F1_released = true;
	}

	return false;
}

bool InputContext::IsF2Toggled() {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_F2] & 0x80) {
		if (m_F2_released) {
			m_F2_released = false;
			return true;
		}
	} else {
		m_F2_released = true;
	}

	return false;
}

bool InputContext::IsF3Toggled() {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_F3] & 0x80) {
		if (m_F3_released) {
			m_F3_released = false;
			return true;
		}
	} else {
		m_F3_released = true;
	}

	return false;
}

bool InputContext::IsF4Toggled() {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_F4] & 0x80) {
		if (m_F4_released) {
			m_F4_released = false;
			return true;
		}
	} else {
		m_F4_released = true;
	}

	return false;
}

bool InputContext::IsF5Toggled() {
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_F5] & 0x80) {
		if (m_F5_released) {
			m_F5_released = false;
			return true;
		}
	} else {
		m_F5_released = true;
	}

	return false;
}
