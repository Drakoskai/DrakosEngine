#pragma once

#include <d3d11_2.h>
#include "ColorShader.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "FontShader.h"
#include "SkydomeShader.h"
#include "TerrainShader.h"
#include "DXMath.h"

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();

	bool Initialize(ID3D11Device*, HWND);
	bool RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix) const;
	bool RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projectionMatrix, ID3D11ShaderResourceView* texture) const;
	bool RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projectionMatrix, ID3D11ShaderResourceView* texture, Vector3 lightDirection, Color diffuse) const;
	bool RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projectionMatrix, ID3D11ShaderResourceView* texture, Color color) const;
	bool RenderSkyDomeShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix, Color apex, Color center) const;
	bool RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView* normalMap2, ID3D11ShaderResourceView* normalMap3, Vector3 lightDirection, Color diffuse) const;

private:
	ShaderManager(const ShaderManager& other);

	ColorShader m_ColorShader;
	TextureShader m_TextureShader;
	LightShader m_LightShader;
	FontShader m_FontShader;
	SkydomeShader m_SkyDomeShader;
	TerrainShader m_TerrainShader;

};
