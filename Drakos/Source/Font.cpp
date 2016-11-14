#include "pch.h"
#include "Font.h"
#include <fstream>
#include <DirectXMath.h>
#include "DXMath.h"
#include "Utility.h"

SimpleFont::SimpleFont() : 
m_Font(nullptr), 
m_Texture(nullptr),
m_fontHeight(0), 
m_spaceSize(0) { }

SimpleFont::SimpleFont(const SimpleFont&) :
m_Font(nullptr), 
m_Texture(nullptr), 
m_fontHeight(0),
m_spaceSize(0) { }

SimpleFont::~SimpleFont()
{
	ReleaseTexture();
	ReleaseFontData();
}

bool SimpleFont::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, char* textureFilename, float fontHeight, int spaceSize)
{
	// Store the height of the font.
	m_fontHeight = fontHeight;

	// Store the size of spaces in pixel size.
	m_spaceSize = spaceSize;

	// Load in the text file containing the font data.
	if (!LoadFontData(fontFilename))
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	if (!LoadTexture(device, deviceContext, textureFilename))
	{
		return false;
	}

	return true;
}

bool SimpleFont::LoadFontData(char* filename)
{	
	// Read in the font size and spacing between chars.
	std::ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	m_Font = new FontType[95];

	for (int i = 0; i < 95; i++)
	{
		char temp;
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

bool SimpleFont::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	m_Texture = new Texture;
	return  m_Texture->Initialize(device, deviceContext, filename);
}

ID3D11ShaderResourceView* SimpleFont::GetTexture() const
{
	return m_Texture->GetTexture();
}

void SimpleFont::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY) const
{
	// Coerce the input vertices into a VertexType structure.
	FontVertexType*  vertexPtr = static_cast<FontVertexType*>(vertices);

	// Get the number of letters in the sentence.
	int numLetters = static_cast<int>(strlen(sentence));

	// Initialize the index to the vertex array.
	int index = 0;

	// Draw each letter onto a quad.
	for (int i = 0; i < numLetters; i++)
	{
		int letter = static_cast<int>(sentence[i]) - 32;
		// If the letter is a space then just move over three pixels.
		if (letter == 0)
		{
			drawX = drawX + static_cast<float>(m_spaceSize);
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = Vector3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = Vector2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = Vector3(drawX + m_Font[letter].size, drawY - m_fontHeight, 0.0f);  // Bottom right.
			vertexPtr[index].texture = Vector2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = Vector3(drawX, drawY - m_fontHeight, 0.0f);  // Bottom left.
			vertexPtr[index].texture = Vector2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = Vector3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = Vector2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = Vector3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = Vector2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = Vector3(drawX + m_Font[letter].size, drawY - m_fontHeight, 0.0f);  // Bottom right.
			vertexPtr[index].texture = Vector2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}

int SimpleFont::GetSentencePixelLength(char* sentence) const
{
	int pixelLength = 0;
	int numLetters = static_cast<int>(strlen(sentence));

	for (int i = 0; i < numLetters; i++)
	{
		int letter = static_cast<int>(sentence[i]) - 32;

		// If the letter is a space then count it as three pixels.
		if (letter == 0)
		{
			pixelLength += m_spaceSize;
		}
		else
		{
			pixelLength += m_Font[letter].size + 1;
		}
	}

	return pixelLength;
}

int SimpleFont::GetFontHeight() const
{
	return static_cast<int>(m_fontHeight);
}

void SimpleFont::ReleaseFontData()
{
	delete m_Font;
	m_Font = nullptr;
}

void SimpleFont::ReleaseTexture()
{
	if (m_Texture)
	{
		delete m_Texture;
		m_Texture = nullptr;
	}
}

