#include "pch.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager() :
	m_ColorShader(),
	m_TextureShader(),
	m_LightShader(),
	m_FontShader(),
	m_SkyDomeShader(),
	m_TerrainShader() {}

ShaderManager::ShaderManager(const ShaderManager&) :
	m_ColorShader(),
	m_TextureShader(),
	m_LightShader(),
	m_FontShader(),
	m_SkyDomeShader(),
	m_TerrainShader() {}

ShaderManager::~ShaderManager() {}

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd) {
	if (!m_ColorShader.Initialize(device, hwnd)) {
		return false;
	}

	if (!m_TextureShader.Initialize(device, hwnd)) {
		return false;
	}

	if (!m_LightShader.Initialize(device, hwnd)) {
		return false;
	}

	if (!m_FontShader.Initialize(device, hwnd)) {
		return false;
	}


	if (!m_SkyDomeShader.Initialize(device, hwnd)) {
		return false;
	}

	if (!m_TerrainShader.Initialize(device, hwnd)) {
		return false;
	}

	return true;
}

bool ShaderManager::RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix) const {
	return m_ColorShader.Render(deviceContext, indexCount, worldMatrix, viewMatrix, projMatrix);
}

bool ShaderManager::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix, ID3D11ShaderResourceView* texture) const {
	return m_TextureShader.Render(deviceContext, indexCount, worldMatrix, viewMatrix, projMatrix, texture);
}

bool ShaderManager::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix, ID3D11ShaderResourceView* texture, Vector3 lightDirection, Color diffuse) const {
	return m_LightShader.Render(deviceContext, indexCount, worldMatrix, viewMatrix, projMatrix, texture, lightDirection, diffuse);
}

bool ShaderManager::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix, ID3D11ShaderResourceView* texture, Color color) const {
	return m_FontShader.Render(deviceContext, indexCount, worldMatrix, viewMatrix, projMatrix, texture, color);
}

bool ShaderManager::RenderSkyDomeShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix, Color apex, Color center) const {
	return m_SkyDomeShader.Render(deviceContext, indexCount, worldMatrix, viewMatrix, projMatrix, apex, center);
}

bool ShaderManager::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView* normalMap2, ID3D11ShaderResourceView* normalMap3, Vector3 lightDirection, Color diffuse) const {
	return m_TerrainShader.Render(deviceContext, indexCount, worldMatrix, viewMatrix, projMatrix, texture, normalMap, normalMap2, normalMap3, lightDirection, diffuse);
}
