#pragma once

#include <d3d11_2.h>
#include "Texture.h"
#include "DXMath.h"

class Bitmap
{
	struct VertexType
	{
		Vector3 position;
		Vector2 texture;
	};

public:
	Bitmap();	
	~Bitmap();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight, char* textureFilename);
	bool Render(ID3D11DeviceContext*, int, int);
	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;

private:
	Bitmap(const Bitmap&);

	bool InitializeBuffers(ID3D11Device*);
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*) const;
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

	int m_vertexCount;
	int m_indexCount;
	int m_screenWidth;
	int m_screenHeight;
	int m_bitmapWidth;
	int m_bitmapHeight;
	int m_previousPosX;
	int m_previousPosY;

	Texture* m_Texture;

};
