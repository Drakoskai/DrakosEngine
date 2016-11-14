#pragma once

#include "Frustum.h"
#include "DXMath.h"

__interface Camera
{
	void SetPosition(Vector3 position);
	void SetRotation(Vector3 rotation);
	Vector3 GetPosition();
	Vector3 GetRotation();
	Matrix GetProjMatrix() const;
	Matrix GetViewMatrix() const;
};

class SimpleCamera
{
public:
	SimpleCamera();	
	~SimpleCamera();

	void SetPosition(const float x, const float y, const float z);
	void SetPosition(Vector3 position);
	void SetRotation(const float x, const float y, const float z);
	void SetRotation(Vector3 rotation);
	Vector3 GetPosition() const;
	Vector3 GetRotation() const;
	void Render();
	void RenderBaseViewMatrix();
	Matrix GetProjMatrix() const;
	Matrix GetViewMatrix() const;

private:
	SimpleCamera(const SimpleCamera&);

	Vector3 mPosition;
	Vector3 mRotation;

	Matrix m_viewMatrix;
	Matrix m_baseViewMatrix;

	Frustum* viewFrustum;
};
