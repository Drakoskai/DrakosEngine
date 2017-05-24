#pragma once
#include <d3d11_2.h>

struct TargaHeader {
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

class Texture {

public:
	Texture();
	~Texture();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	ID3D11ShaderResourceView* GetTexture() const;

private:
	Texture(const Texture&);

	bool LoadTarga(char* filename, int& height, int& width);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;

	unsigned char* m_targaData;

};
