#pragma once

#include <d3d11_2.h>
#include "DXMath.h"

class SkyDomeModel
{

	struct ModelType
	{
		float x;
		float y;
		float z;
		float tu;
		float tv;
		float nx;
		float ny;
		float nz;
	};

	struct VertexType
	{
		Vector3 position;
	};

public:
	SkyDomeModel();
	~SkyDomeModel();

	bool Initialize(ID3D11Device*);
	void Render(ID3D11DeviceContext*) const;
	int GetIndexCount() const;
	Color GetApexColor() const;
	Color GetCenterColor() const;

private:
	SkyDomeModel(const SkyDomeModel&);

	bool Load(char*);
	bool InitializeBuffers(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext*) const;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	
	int m_vertexCount;
	int m_indexCount;

	Color m_apexColor;
	Color m_centerColor;

	ModelType* m_model;

};
