#include "pch.h"
#include "TextureShader.h"
#include "DXHelper.h"

TextureShader::TextureShader() :
m_vertexShader(nullptr),
m_pixelShader(nullptr),
m_layout(nullptr),
m_matrixBuffer(nullptr),
m_sampleState(nullptr) { }

TextureShader::TextureShader(const TextureShader&) :
m_vertexShader(nullptr),
m_pixelShader(nullptr),
m_layout(nullptr),
m_matrixBuffer(nullptr),
m_sampleState(nullptr) { }

TextureShader::~TextureShader() { }

bool TextureShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	return InitializeShader(device, hwnd, L"../Drakos/Source/Shaders/texture.vs", L"../Drakos/Source/Shaders/texture.ps");
}

bool TextureShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projectionMatrix, ID3D11ShaderResourceView* texture) const
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture))
	{
		return false;
	}

	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout.Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return true;
}

bool TextureShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	// Initialize the pointers this function will use to null.
	Microsoft::WRL::ComPtr<ID3D10Blob> errorMessage = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> vertexShaderBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> pixelShaderBuffer = nullptr;

	// Compile the vertex shader code.
	HRESULT result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			DX::OutputShaderErrorMessage(errorMessage.Get(), hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, nullptr, nullptr, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			DX::OutputShaderErrorMessage(errorMessage.Get(), hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), m_layout.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, m_matrixBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, m_sampleState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, Matrix worldMatrix, Matrix viewMatrix, Matrix projMatrix, ID3D11ShaderResourceView* texture) const
{
	// Transpose the matrices to prepare them for the shader.
	worldMatrix.Transpose();
	viewMatrix.Transpose();
	projMatrix.Transpose();
	/*worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projMatrix = XMMatrixTranspose(projMatrix);*/

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}
