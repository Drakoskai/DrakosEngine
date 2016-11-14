#pragma once
#include "DXMath.h"
#include "InputContext.h"

class CameraController
{
public:
	CameraController();
	CameraController(const CameraController& other);
	~CameraController();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void GetPosition(float& x, float& y, float& z) const;
	Vector3 GetPosition() const;
	void GetRotation(float& x, float& y, float& z) const;
	Vector3 GetRotation() const;
	void SetFrameTime(float time);
	void KeyPressed(KeyEvent e);
	void MoveUpward(bool keydown);
	void MoveDownward(bool keydown);
	void LookUpward(bool keydown);
	void LookDownward(bool keydown);

private:
	void MoveForward(bool keydown);
	void MoveBackward(bool keydown);
	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);
	float m_positionX;
	float m_positionY;
	float m_positionZ;
	float m_rotationX;
	float m_rotationY;
	float m_rotationZ;

	float m_frameTime;

	float m_forwardSpeed;
	float m_backwardSpeed;
	float m_upwardSpeed;
	float m_downwardSpeed;
	float m_leftTurnSpeed;
	float m_rightTurnSpeed;
	float m_lookUpSpeed;
	float m_lookDownSpeed;
};
