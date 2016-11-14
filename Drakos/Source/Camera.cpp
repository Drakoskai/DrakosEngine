#include "pch.h"
#include "Camera.h"

SimpleCamera::SimpleCamera() :
viewFrustum(nullptr) { }

SimpleCamera::SimpleCamera(const SimpleCamera&) :
viewFrustum(nullptr) { }

SimpleCamera::~SimpleCamera() { }

void SimpleCamera::SetPosition(const float x, const float y, const float z)
{
	SetPosition(Vector3(x, y, z));
}

void SimpleCamera::SetPosition(const Vector3 position)
{
	mPosition = position;
}

void SimpleCamera::SetRotation(const float x, const float y, const float z)
{
	SetRotation(Vector3(x, y, z));
}

void SimpleCamera::SetRotation(const Vector3 rotation)
{
	mRotation = rotation;
}

Vector3 SimpleCamera::GetPosition() const
{
	return mPosition;
}

Vector3 SimpleCamera::GetRotation() const
{
	return mRotation;
}

void SimpleCamera::Render()
{
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = mRotation.x * 0.0174532925f;
	float yaw = mRotation.y * 0.0174532925f;
	float roll = mRotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	Vector3 lookAtVector = Vector3::UnitZ;
	lookAtVector = Vector3::Transform(lookAtVector, rotationMatrix);
	Vector3 upVector = Vector3::Up;
	upVector = Vector3::Transform(upVector, rotationMatrix); 

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector += mPosition;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = Matrix::CreateLookAt(mPosition, lookAtVector, upVector);
}

Matrix SimpleCamera::GetProjMatrix() const
{
	return m_viewMatrix;
}

void SimpleCamera::RenderBaseViewMatrix()
{
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = mRotation.x * 0.0174532925f;
	float yaw = mRotation.y * 0.0174532925f;
	float roll = mRotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	Vector3 lookAtVector = Vector3::UnitZ;
	lookAtVector = Vector3::Transform(lookAtVector, rotationMatrix);
	Vector3 upVector = Vector3::Up;
	upVector = Vector3::Transform(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector += mPosition; 

	// Finally create the view matrix from the three updated vectors.
	m_baseViewMatrix = Matrix::CreateLookAt(mPosition, lookAtVector, upVector);
}

Matrix SimpleCamera::GetViewMatrix() const
{
	return m_baseViewMatrix;
}
