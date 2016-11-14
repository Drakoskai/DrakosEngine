#pragma once

struct KeyEvent
{
	bool aKey;
	bool dKey;
	bool sKey;
	bool wKey;
	bool directionUp;
	bool directionDown;
	bool directionLeft;
	bool directionRight;
};

struct MouseEvent
{
	bool buttonOne;
	bool buttonTwo;
	bool buttonThree;
	bool mouseClicked;
	bool mouseDragged;
	bool mouseEntered;
	bool mouseExited;
	bool mouseMoved;
	bool mousePressed;
	bool mouseReleased;
};

__interface KeyListener
{
	void KeyPressed(KeyEvent e);
	void KeyReleased(KeyEvent e);
	void KeyTyped(KeyEvent e);
};

__interface MouseListener
{
	void MouseClick(MouseEvent e);
	void MouseEntered(MouseEvent e);
	void MouseExited(MouseEvent e);
	void MousePressed(MouseEvent e);
	void MouseReleased(MouseEvent e);
};
