#pragma once

#include "Bitmap.h"
#include "ShaderManager.h"
#include "DXMath.h"

class Minimap {
public:
	Minimap();
	~Minimap();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, float terrainWidth, float terrainHeight);
	bool Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, Matrix worldMatrix, Matrix viewMatrix, Matrix orthoMatrix) const;
	void PositionUpdate(float, float);

private:
	Minimap(const Minimap&);

	int m_mapLocationX;
	int m_mapLocationY;
	int m_pointLocationX;
	int m_pointLocationY;
	float m_mapSizeX;
	float m_mapSizeY;
	float m_terrainWidth;
	float m_terrainHeight;
	Bitmap *m_MiniMapBitmap;
	Bitmap *m_PointBitmap;

};
