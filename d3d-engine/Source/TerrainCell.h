#pragma once

#include <d3d11_2.h>
#include "DXMath.h"

class TerrainCell {
	struct VectorType {
		float x;
		float y;
		float z;
	};

	struct ModelType {
		float x;
		float y;
		float z;
		float tu;
		float tv;
		float nx;
		float ny;
		float nz;
		float tx;
		float ty;
		float tz;
		float bx;
		float by;
		float bz;
		float r;
		float g;
		float b;
		float tu2;
		float tv2;
	};

	struct ColorVertexType {
		Vector3 position;
		Color color;
	};

	struct VertexType {
		Vector3 position;
		Vector2 texture;
		Vector3 normal;
		Vector3 tangent;
		Vector3 binormal;
		Color color;
		Vector2 texture2;
	};

public:
	TerrainCell();
	~TerrainCell();

	bool Initialize(ID3D11Device* device, void* terrainModelPtr, int nodeIndexX, int nodeIndexY, int cellHeight, int cellWidth, int terrainWidth);
	void Render(ID3D11DeviceContext* deviceContext) const;
	void RenderLineBuffers(ID3D11DeviceContext* deviceContext) const;

	int GetVertexCount() const;
	int GetIndexCount() const;
	int GetLineBuffersIndexCount() const;
	void GetCellDimensions(float& maxWidth, float& maxHeight, float& maxDepth, float& minWidth, float& minHeight, float& minDepth) const;

	VectorType* m_vertexList;
private:
	TerrainCell(const TerrainCell&);
	bool InitializeBuffers(ID3D11Device* device, int nodeIndexX, int nodeIndexY, int cellHeight, int cellWidth, int terrainWidth, ModelType* terrainModel);
	void RenderBuffers(ID3D11DeviceContext*) const;
	void CalculateCellDimensions();
	bool BuildLineBuffers(ID3D11Device*);

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_lineVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_lineIndexBuffer;

	int m_vertexCount;
	int m_indexCount;
	int m_lineIndexCount;

	float m_maxWidth;
	float m_maxHeight;
	float m_maxDepth;
	float m_minWidth;
	float m_minHeight;
	float m_minDepth;

	float m_positionX;
	float m_positionY;
	float m_positionZ;

};
