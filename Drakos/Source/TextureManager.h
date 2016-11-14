#pragma once

#include <d3d11_2.h>
#include "Texture.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	bool Initialize(int);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*, int) const;
	ID3D11ShaderResourceView* GetTexture(int) const;

private:
	TextureManager(const TextureManager&);

	Texture* m_TextureArray;
	int m_textureCount;
	
};
