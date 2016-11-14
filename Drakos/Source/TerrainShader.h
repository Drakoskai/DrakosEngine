#pragma once

#include "DXMath.h"

class TerrainShader
{
	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	struct LightBufferType
	{
		Color diffuseColor;
		Vector3 lightDirection;
		float padding;
	};

public:
	TerrainShader();
	~TerrainShader();

	bool Initialize(ID3D11Device*, HWND);
	bool Render(ID3D11DeviceContext*, int, Matrix, Matrix, Matrix, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, Vector3, Color) const;
	
private:
	TerrainShader(const TerrainShader&);

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);	
	bool SetShaderParameters(ID3D11DeviceContext*, Matrix, Matrix, Matrix, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, Vector3, Color) const;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampleState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_lightBuffer;
};
