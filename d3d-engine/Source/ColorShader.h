#pragma once

#include <d3d11_2.h>
#include "DXMath.h"

class ColorShader {
	struct MatrixBufferType {
		Matrix world;
		Matrix view;
		Matrix projection;
	};

public:
	ColorShader();
	~ColorShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projectionMatrix) const;

private:
	ColorShader(const ColorShader& other);
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	bool SetShaderParameters(ID3D11DeviceContext*, Matrix, Matrix, Matrix) const;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
};
