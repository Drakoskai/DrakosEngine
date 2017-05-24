#include "pch.h"
#include "Bitmap.h"
#include "Utility.h"

Bitmap::Bitmap() :
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_vertexCount(0),
	m_indexCount(0),
	m_screenWidth(0),
	m_screenHeight(0),
	m_bitmapWidth(0),
	m_bitmapHeight(0),
	m_previousPosX(0),
	m_previousPosY(0),
	m_Texture(nullptr) {}

Bitmap::Bitmap(const Bitmap&) :
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_vertexCount(0),
	m_indexCount(0),
	m_screenWidth(0),
	m_screenHeight(0),
	m_bitmapWidth(0),
	m_bitmapHeight(0),
	m_previousPosX(0),
	m_previousPosY(0),
	m_Texture(nullptr) {}

Bitmap::~Bitmap() {
	if (m_Texture) {
		delete m_Texture;
		m_Texture = nullptr;
	}
}

bool Bitmap::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight, char* textureFilename) {
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;

	if (!InitializeBuffers(device)) {
		return false;
	}

	return LoadTexture(device, deviceContext, textureFilename);
}

bool Bitmap::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY) {
	if (!UpdateBuffers(deviceContext, positionX, positionY)) {
		return false;
	}

	RenderBuffers(deviceContext);

	return true;
}

int Bitmap::GetIndexCount() const {
	return m_indexCount;
}

ID3D11ShaderResourceView* Bitmap::GetTexture() const {
	return m_Texture->GetTexture();
}

bool Bitmap::InitializeBuffers(ID3D11Device* device) {
	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	VertexType* vertices = new VertexType[m_vertexCount];

	// Create the index array.
	unsigned long* indices = new unsigned long[m_indexCount];

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, sizeof(VertexType) * m_vertexCount);

	// Load the index array with data.
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

	// Now finally create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	// Set up the description of the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

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
		return false;
	}

	delete[] vertices;
	delete[] indices;

	return true;
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext* deviceContent, int positionX, int positionY) {
	// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.
	if (positionX == m_previousPosX && positionY == m_previousPosY) {
		return true;
	}

	// If it has changed then update the position it is being rendered to.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// Calculate the screen coordinates of the left side of the bitmap.
	float left = static_cast<float>(m_screenWidth / 2 * -1) + static_cast<float>(positionX);

	// Calculate the screen coordinates of the right side of the bitmap.
	float right = left + static_cast<float>(m_bitmapWidth);

	// Calculate the screen coordinates of the top of the bitmap.
	float top = static_cast<float>(m_screenHeight / 2) - static_cast<float>(positionY);

	// Calculate the screen coordinates of the bottom of the bitmap.
	float bottom = top - static_cast<float>(m_bitmapHeight);

	// Create the vertex array.
	VertexType* vertices = new VertexType[m_vertexCount];

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = Vector3(left, top, 0.0f);  // Top left.
	vertices[0].texture = Vector2(0.0f, 0.0f);

	vertices[1].position = Vector3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = Vector2(1.0f, 1.0f);

	vertices[2].position = Vector3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = Vector2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = Vector3(left, top, 0.0f);  // Top left.
	vertices[3].texture = Vector2(0.0f, 0.0f);

	vertices[4].position = Vector3(right, top, 0.0f);  // Top right.
	vertices[4].texture = Vector2(1.0f, 0.0f);

	vertices[5].position = Vector3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = Vector2(1.0f, 1.0f);

	// Lock the vertex buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	HRESULT result = deviceContent->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		delete[] vertices;
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	VertexType* dataPtr = static_cast<VertexType*>(mappedResource.pData);

	// Copy the data into the vertex buffer.
	memcpy(dataPtr, static_cast<void*>(vertices), sizeof(VertexType) * m_vertexCount);

	// Unlock the vertex buffer.
	deviceContent->Unmap(m_vertexBuffer.Get(), 0);
	delete[] vertices;

	return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext* deviceContext) const {
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Bitmap::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {
	m_Texture = new Texture;
	return  m_Texture->Initialize(device, deviceContext, filename);
}
