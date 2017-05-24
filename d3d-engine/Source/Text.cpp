#include "pch.h"
#include "Text.h"
#include "Utility.h"

Text::Text() :
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_shadowVertexBuffer(nullptr),
	m_shadowIndexBuffer(nullptr),
	m_screenWidth(0),
	m_screenHeight(0),
	m_maxLength(0),
	m_vertexCount(0),
	m_indexCount(0),
	m_shadow(false) {}

Text::Text(const Text&) :
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_shadowVertexBuffer(nullptr),
	m_shadowIndexBuffer(nullptr),
	m_screenWidth(0),
	m_screenHeight(0),
	m_maxLength(0),
	m_vertexCount(0),
	m_indexCount(0),
	m_shadow(false) {}

Text::~Text() {}

bool Text::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, int maxLength, bool shadow, SimpleFont* Font, char* text, int positionX, int positionY, float red, float green, float blue) {
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_maxLength = maxLength;
	m_shadow = shadow;

	return InitializeSentence(device, deviceContext, Font, text, positionX, positionY, red, green, blue);
}

void Text::Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, Matrix worldMatrix, Matrix viewMatrix, Matrix orthoMatrix, ID3D11ShaderResourceView* fontTexture) const {
	RenderSentence(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, fontTexture);
}

bool Text::InitializeSentence(ID3D11Device* device, ID3D11DeviceContext* deviceContext, SimpleFont* Font, char* text, int positionX, int positionY, float red, float green, float blue) {
	// Set the vertex and index count.
	m_vertexCount = 6 * m_maxLength;
	m_indexCount = 6 * m_maxLength;

	// Create the vertex array.
	VertexType* vertices = new VertexType[m_vertexCount];

	// Create the index array.
	unsigned long* indices = new unsigned long[m_indexCount];

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, sizeof(VertexType) * m_vertexCount);

	// Initialize the index array.
	for (int i = 0; i < m_indexCount; i++) {
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc = {};

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf());
	if (FAILED(result)) {
		delete[] vertices;
		delete[] indices;
		return false;
	}

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc = {};

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData = {};

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf());
	if (FAILED(result)) {
		delete[] vertices;
		delete[] indices;
		return false;
	}

	// If shadowed create the second vertex and index buffer.
	if (m_shadow) {
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_shadowVertexBuffer.GetAddressOf());
		if (FAILED(result)) {
			delete[] vertices;
			delete[] indices;
			return false;
		}

		result = device->CreateBuffer(&indexBufferDesc, &indexData, m_shadowIndexBuffer.GetAddressOf());
		if (FAILED(result)) {
			delete[] vertices;
			delete[] indices;
			return false;
		}
	}


	delete[] vertices;
	delete[] indices;

	// Now add the text data to the sentence buffers.
	return UpdateSentence(deviceContext, Font, text, positionX, positionY, red, green, blue);
}

bool Text::UpdateSentence(ID3D11DeviceContext* deviceContext, SimpleFont* Font, char* text, int positionX, int positionY, float red, float green, float blue) {
	// Store the color of the sentence.
	m_pixelColor = Color(red, green, blue, 1.0f);

	// Get the number of letters in the sentence.
	int numLetters = static_cast<int>(strlen(text));

	// Check for possible buffer overflow.
	if (numLetters > m_maxLength) {
		return false;
	}

	VertexType* vertices = new VertexType[m_vertexCount];
	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	float drawX = static_cast<float>(m_screenWidth / 2 * -1 + positionX);
	float drawY = static_cast<float>(m_screenHeight / 2 - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	Font->BuildVertexArray(static_cast<void*>(vertices), text, drawX, drawY);

	// Lock the vertex buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		delete[] vertices;
		return false;
	}

	// Get a pointer to the mapped resource data pointer.
	void* verticesPtr;
	verticesPtr = static_cast<void*>(mappedResource.pData);

	// Copy the vertex array into the vertex buffer.
	memcpy(verticesPtr, static_cast<void*>(vertices), (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer.Get(), 0);

	// If shadowed then do the same for the second vertex buffer but offset by two pixels on both axis.
	if (m_shadow) {
		memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

		drawX = static_cast<float>((((m_screenWidth / 2) * -1) + positionX) + 2);
		drawY = static_cast<float>(((m_screenHeight / 2) - positionY) - 2);
		Font->BuildVertexArray(static_cast<void*>(vertices), text, drawX, drawY);

		result = deviceContext->Map(m_shadowVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) {
			delete[] vertices;
			return false;
		}

		verticesPtr = static_cast<void*>(mappedResource.pData);
		memcpy(verticesPtr, static_cast<void*>(vertices), (sizeof(VertexType) * m_vertexCount));
		deviceContext->Unmap(m_shadowVertexBuffer.Get(), 0);
	}

	delete[] vertices;

	return true;
}

void Text::RenderSentence(ID3D11DeviceContext* deviceContext, ShaderManager* ShaderManager, Matrix worldMatrix, Matrix viewMatrix, Matrix orthoMatrix, ID3D11ShaderResourceView* fontTexture) const {
	// Set vertex buffer stride and offset.
	Color shadowColor;
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// If shadowed then render the shadow text first.
	if (m_shadow) {
		shadowColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
		deviceContext->IASetVertexBuffers(0, 1, m_shadowVertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(m_shadowIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ShaderManager->RenderFontShader(deviceContext, m_indexCount, worldMatrix, viewMatrix, orthoMatrix, fontTexture, shadowColor);
	}

	// Render the text buffers.
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ShaderManager->RenderFontShader(deviceContext, m_indexCount, worldMatrix, viewMatrix, orthoMatrix, fontTexture, m_pixelColor);
}
