#pragma once

#include "DXMath.h"

class Light
{
public:
	Light();	
	~Light();

	void SetAmbientColor(float r, float g, float b, float a);
	void SetAmbientColor(Color ambient);

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDiffuseColor(Color diffuse);

	void SetDirection(float x, float y, float z);
	void SetDirection(Vector3 direction);

	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3 position);

	Color GetAmbientColor() const;
	Color GetDiffuseColor() const;
	Vector3 GetDirection() const;
	Vector3 GetPosition() const;

private:
	Light(const Light&);

	Color m_ambient;
	Color m_diffuse;
	Vector3 m_direction;
	Vector3 m_position;

};
