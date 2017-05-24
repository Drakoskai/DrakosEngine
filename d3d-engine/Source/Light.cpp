#include "pch.h"
#include "Light.h"

Light::Light() {}

Light::Light(const Light&) {}

Light::~Light() {}

void Light::SetAmbientColor(float r, float g, float b, float a) {
	SetAmbientColor(Color(r, g, b, a));
}

void Light::SetAmbientColor(Color ambient) {
	m_ambient = ambient;
}

void Light::SetDiffuseColor(float r, float g, float b, float a) {
	SetDiffuseColor(Color(r, g, b, a));
}

void Light::SetDiffuseColor(Color diffuse) {
	m_diffuse = diffuse;
}

void Light::SetDirection(float x, float y, float z) {
	SetDirection(Vector3(x, y, z));
}

void Light::SetDirection(Vector3 direction) {
	m_direction = direction;
}

void Light::SetPosition(float x, float y, float z) {
	SetPosition(Vector3(x, y, z));
}

void Light::SetPosition(Vector3 position) {
	m_position = position;
}

Color Light::GetAmbientColor() const {
	return m_ambient;
}

Color Light::GetDiffuseColor() const {
	return m_diffuse;
}

Vector3 Light::GetDirection() const {
	return m_direction;
}

Vector3 Light::GetPosition() const {
	return m_position;
}
