#pragma once

#include <d3d11_2.h>
#include "Texture.h"
#include "DXMath.h"

class SimpleFont {
	struct FontType {
		float left;
		float right;
		int size;
	};

	struct FontVertexType {
		Vector3 position;
		Vector2 texture;
	};

public:
	SimpleFont();
	~SimpleFont();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*, float, int);
	ID3D11ShaderResourceView* GetTexture() const;
	void BuildVertexArray(void*, char*, float, float) const;
	int GetSentencePixelLength(char*) const;
	int GetFontHeight() const;

private:
	SimpleFont(const SimpleFont&);

	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	FontType* m_Font;
	Texture* m_Texture;
	float m_fontHeight;
	int m_spaceSize;

};
