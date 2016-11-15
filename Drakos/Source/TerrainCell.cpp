#include "pch.h"
#include "TerrainCell.h"
#include "Utility.h"

TerrainCell::TerrainCell() :
m_vertexList(nullptr),
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_lineVertexBuffer(nullptr),
m_lineIndexBuffer(nullptr),
m_vertexCount(0),
m_indexCount(0),
m_lineIndexCount(0),
m_maxWidth(0),
m_maxHeight(0),
m_maxDepth(0),
m_minWidth(0),
m_minHeight(0),
m_minDepth(0),
m_positionX(0),
m_positionY(0),
m_positionZ(0) { }

TerrainCell::TerrainCell(const TerrainCell&) :
m_vertexList(nullptr),
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_lineVertexBuffer(nullptr),
m_lineIndexBuffer(nullptr),
m_vertexCount(0),
m_indexCount(0),
m_lineIndexCount(0),
m_maxWidth(0),
m_maxHeight(0),
m_maxDepth(0),
m_minWidth(0),
m_minHeight(0),
m_minDepth(0),
m_positionX(0),
m_positionY(0),
m_positionZ(0)
{ }

TerrainCell::~TerrainCell()
{
	delete[] m_vertexList;
	m_vertexList = nullptr;
}

bool TerrainCell::Initialize(ID3D11Device* device, void* terrainModelPtr, int nodeIndexX, int nodeIndexY, int cellHeight, int cellWidth, int terrainWidth)
{
	// Coerce the pointer to the terrain model into the model type.
	ModelType* terrainModel = static_cast<ModelType*>(terrainModelPtr);

	// Load the rendering buffers with the terrain data for this cell index.
	HRESULT result = InitializeBuffers(device, nodeIndexX, nodeIndexY, cellHeight, cellWidth, terrainWidth, terrainModel);
	if (FAILED(result))
	{
		return false;
	}

	CalculateCellDimensions();

	return BuildLineBuffers(device);
}

void TerrainCell::Render(ID3D11DeviceContext* deviceContext) const
{
	RenderBuffers(deviceContext);
}

int TerrainCell::GetVertexCount() const
{
	return m_vertexCount;
}

int TerrainCell::GetIndexCount() const
{
	return m_indexCount;
}

bool TerrainCell::InitializeBuffers(ID3D11Device* device, int nodeIndexX, int nodeIndexY, int cellHeight, int cellWidth, int terrainWidth, ModelType* terrainModel)
{
	// Calculate the number of vertices in this terrain cell.
	m_vertexCount = (cellHeight - 1) * (cellWidth - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	VertexType* vertices = new VertexType[m_vertexCount];
	unsigned long* indices = new unsigned long[m_indexCount];

	// Setup the indexes into the terrain model data and the local vertex/index array.
	int modelIndex = (nodeIndexX * (cellWidth - 1) + nodeIndexY * (cellHeight - 1) * (terrainWidth - 1)) * 6;
	int index = 0;

	// Load the vertex array and index array with data.
	for (int j = 0; j < cellHeight - 1; j++)
	{
		for (int i = 0; i < ((cellWidth - 1) * 6); i++)
		{
			vertices[index].position = Vector3(terrainModel[modelIndex].x, terrainModel[modelIndex].y, terrainModel[modelIndex].z);
			vertices[index].texture = Vector2(terrainModel[modelIndex].tu, terrainModel[modelIndex].tv);
			vertices[index].normal = Vector3(terrainModel[modelIndex].nx, terrainModel[modelIndex].ny, terrainModel[modelIndex].nz);
			vertices[index].tangent = Vector3(terrainModel[modelIndex].tx, terrainModel[modelIndex].ty, terrainModel[modelIndex].tz);
			vertices[index].binormal = Vector3(terrainModel[modelIndex].bx, terrainModel[modelIndex].by, terrainModel[modelIndex].bz);
			vertices[index].color = Color(terrainModel[modelIndex].r, terrainModel[modelIndex].g, terrainModel[modelIndex].b);
			vertices[index].texture2 = Vector2(terrainModel[modelIndex].tu2, terrainModel[modelIndex].tv2);
			indices[index] = index;
			modelIndex++;
			index++;
		}
		modelIndex += (terrainWidth * 6) - (cellWidth * 6);
	}

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc = {};

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData = {};

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf());
	if (FAILED(result))
	{
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
	if (FAILED(result))
	{
		delete[] vertices;
		delete[] indices;
		return false;
	}

	// Create a public vertex array that will be used for accessing vertex information about this cell.
	m_vertexList = new VectorType[m_vertexCount];

	// Keep a local copy of the vertex position data for this cell.
	for (int i = 0; i < m_vertexCount; i++)
	{
		m_vertexList[i].x = vertices[i].position.x;
		m_vertexList[i].y = vertices[i].position.y;
		m_vertexList[i].z = vertices[i].position.z;
	}

	delete[] vertices;
	delete[] indices;

	return true;
}

void TerrainCell::RenderBuffers(ID3D11DeviceContext* deviceContext) const
{
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

void TerrainCell::CalculateCellDimensions()
{
	// Initialize the dimensions of the node.
	m_maxWidth = -1000000.0f;
	m_maxHeight = -1000000.0f;
	m_maxDepth = -1000000.0f;

	m_minWidth = 1000000.0f;
	m_minHeight = 1000000.0f;
	m_minDepth = 1000000.0f;

	for (int i = 0; i < m_vertexCount; i++)
	{
		float width = m_vertexList[i].x;
		float height = m_vertexList[i].y;
		float depth = m_vertexList[i].z;

		// Check if the width exceeds the minimum or maximum.
		if (width > m_maxWidth)
		{
			m_maxWidth = width;
		}
		if (width < m_minWidth)
		{
			m_minWidth = width;
		}

		// Check if the height exceeds the minimum or maximum.
		if (height > m_maxHeight)
		{
			m_maxHeight = height;
		}
		if (height < m_minHeight)
		{
			m_minHeight = height;
		}

		// Check if the depth exceeds the minimum or maximum.
		if (depth > m_maxDepth)
		{
			m_maxDepth = depth;
		}
		if (depth < m_minDepth)
		{
			m_minDepth = depth;
		}
	}

	// Calculate the center position of this cell.
	m_positionX = (m_maxWidth - m_minWidth) + m_minWidth;
	m_positionY = (m_maxHeight - m_minHeight) + m_minHeight;
	m_positionZ = (m_maxDepth - m_minDepth) + m_minDepth;
}

bool TerrainCell::BuildLineBuffers(ID3D11Device* device)
{
	// Set the color of the lines to orange.
	Color lineColor = Color(1.0f, 0.5f, 0.0f, 1.0f);

	int vertexCount = 24;
	int indexCount = vertexCount;

	ColorVertexType*  vertices = new ColorVertexType[vertexCount];

	unsigned long*  indices = new unsigned long[indexCount];

	// Set up the description of the vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc = {};

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColorVertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData = {};

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Set up the description of the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc = {};

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData = {};

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Load the vertex and index array with data.
	int index = 0;

	// 8 Horizontal lines.
	vertices[index].position = Vector3(m_minWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	// 4 Verticle lines.
	vertices[index].position = Vector3(m_maxWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_maxHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_minHeight, m_maxDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_maxWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_maxHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;
	index++;

	vertices[index].position = Vector3(m_minWidth, m_minHeight, m_minDepth);
	vertices[index].color = lineColor;
	indices[index] = index;

	// Create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_lineVertexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		delete[] vertices;
		delete[] indices;
		return false;
	}

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, m_lineIndexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		delete[] vertices;
		delete[] indices;
		return false;
	}
	// Store the index count for rendering.
	m_lineIndexCount = indexCount;

	delete[] vertices;
	delete[] indices;

	return true;
}

void TerrainCell::RenderLineBuffers(ID3D11DeviceContext* deviceContext) const
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(ColorVertexType);
	unsigned int offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, m_lineVertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_lineIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case lines.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

int TerrainCell::GetLineBuffersIndexCount() const
{
	return m_lineIndexCount;
}

void TerrainCell::GetCellDimensions(float& maxWidth, float& maxHeight, float& maxDepth, float& minWidth, float& minHeight, float& minDepth) const
{
	maxWidth = m_maxWidth;
	maxHeight = m_maxHeight;
	maxDepth = m_maxDepth;
	minWidth = m_minWidth;
	minHeight = m_minHeight;
	minDepth = m_minDepth;
}
