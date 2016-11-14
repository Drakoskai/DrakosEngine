#include "pch.h"
#include "TextureManager.h"

TextureManager::TextureManager() : 
m_TextureArray(nullptr),
m_textureCount(0) { }

TextureManager::TextureManager(const TextureManager&) :
m_TextureArray(nullptr), 
m_textureCount(0) { }

TextureManager::~TextureManager()
{
	if (m_TextureArray)
	{
		delete[] m_TextureArray;
		m_TextureArray = nullptr;
	}
}

bool TextureManager::Initialize(int count)
{
	m_textureCount = count;

	// Create the color texture object.
	m_TextureArray = new Texture[m_textureCount];
	if (!m_TextureArray)
	{
		return false;
	}

	return true;
}

bool TextureManager::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, int location) const
{
	// Initialize the color texture object.
	return m_TextureArray[location].Initialize(device, deviceContext, filename);
}

ID3D11ShaderResourceView* TextureManager::GetTexture(int id) const
{
	return m_TextureArray[id].GetTexture();
}
