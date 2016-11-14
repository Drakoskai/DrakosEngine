#include "pch.h"
#include "Texture.h"
#include "Utility.h"

Texture::Texture() :
m_texture(nullptr),
m_textureView(nullptr),
m_targaData(nullptr) { }

Texture::Texture(const Texture&) :
m_texture(nullptr),
m_textureView(nullptr),
m_targaData(nullptr) { }

Texture::~Texture() { }

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	// Load the targa image data into memory.
	int height;
	int width;
	
	if (!LoadTarga(filename, height, width))
	{
		return false;
	}

	// Setup the description of the texture.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	HRESULT result = device->CreateTexture2D(&textureDesc, nullptr, m_texture.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	unsigned int rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_texture.Get(), 0, nullptr, m_targaData, rowPitch, 0);
	// Setup the shader resource view description.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture
	result = device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_textureView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_textureView.Get());

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] m_targaData;
	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture() const
{
	return m_textureView.Get();
}

bool Texture::LoadTarga(char* filename, int& height, int& width)
{
	// Open the targa file for reading in binary.
	FILE* filePtr;
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	TargaHeader targaFileHeader;
	unsigned int  count = static_cast<unsigned int>(fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr));
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	height = static_cast<int>(targaFileHeader.height);
	width = static_cast<int>(targaFileHeader.width);
	int bpp = static_cast<int>(targaFileHeader.bpp);

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	unsigned int imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	unsigned char* targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	// Read in the targa image data.
	count = static_cast<unsigned int>(fread(targaImage, 1, imageSize, filePtr));
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Allocate memory for the targa destination data.
	m_targaData = new unsigned char[imageSize];
	if (!m_targaData)
	{
		return false;
	}
	// Initialize the index into the targa destination data array.
	int index = 0;

	// Initialize the index into the targa image data.
	int k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			m_targaData[index + 0] = targaImage[k + 2];  // Red.
			m_targaData[index + 1] = targaImage[k + 1];  // Green.
			m_targaData[index + 2] = targaImage[k + 0];  // Blue
			m_targaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;

	return true;
}