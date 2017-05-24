#include "pch.h"
#include "SkydomeShader.h"
#include "DXHelper.h"

SkydomeShader::SkydomeShader() :
	m_vertexShader(nullptr),
	m_pixelShader(nullptr),
	m_layout(nullptr),
	m_matrixBuffer(nullptr),
	m_colorBuffer(nullptr) {}

SkydomeShader::SkydomeShader(const SkydomeShader&) :
	m_vertexShader(nullptr),
	m_pixelShader(nullptr),
	m_layout(nullptr),
	m_matrixBuffer(nullptr),
	m_colorBuffer(nullptr) {}

SkydomeShader::~SkydomeShader() {}

bool SkydomeShader::Initialize(ID3D11Device* device, HWND hwnd) {
	return InitializeShader(device, hwnd, L"../d3d-engine/Source/Shaders/skydome.vs", L"../d3d-engine/Source/Shaders/skydome.ps");
}

bool SkydomeShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, Matrix worldMatrix, Matrix viewMatrix, Matrix projectionMatrix, Color apexColor, Color centerColor) const {
	// Set the shader parameters that it will use for rendering.
	bool result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
	if (!result) {
		return false;
	}

	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout.Get());

	// Set the vertex and pixel shaders that will be used to render the triangles.
	deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// Render the triangles.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return true;
}

bool SkydomeShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) {
	Microsoft::WRL::ComPtr<ID3D10Blob> errorMessage = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> vertexShaderBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> pixelShaderBuffer = nullptr;

	// Compile the vertex shader code.
	HRESULT result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "SkyDomeVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
	if (FAILED(result)) {
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage) {
			DX::OutputShaderErrorMessage(errorMessage.Get(), hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else {
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, nullptr, nullptr, "SkyDomePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
	if (FAILED(result)) {
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage) {
			DX::OutputShaderErrorMessage(errorMessage.Get(), hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else {
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	// Create the vertex input layout description.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), m_layout.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the dynamic constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc = {};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, m_matrixBuffer.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the dynamic pixel constant buffer that is in the pixel shader.
	D3D11_BUFFER_DESC colorBufferDesc = {};
	colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	colorBufferDesc.ByteWidth = sizeof(ColorBufferType);
	colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colorBufferDesc.MiscFlags = 0;
	colorBufferDesc.StructureByteStride = 0;

	// Create the pixel constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&colorBufferDesc, nullptr, m_colorBuffer.GetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	return true;
}

bool SkydomeShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, Matrix worldMatrix, Matrix viewMatrix, Matrix projectionMatrix, Color apexColor, Color centerColor) const {
	// Transpose the matrices to prepare them for the shader.
	/*worldMatrix.Transpose();
	viewMatrix.Transpose();
	projectionMatrix.Transpose();*/
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the matrix constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	// Get a pointer to the data in the matrix constant buffer.
	MatrixBufferType* dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	// Copy the matrices into the matrix constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the matrix constant buffer.
	deviceContext->Unmap(m_matrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	// Now set the matrix constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

	// Lock the color constant buffer so it can be written to.
	result = deviceContext->Map(m_colorBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	// Get a pointer to the data in the color constant buffer.
	ColorBufferType* colorDataPtr = static_cast<ColorBufferType*>(mappedResource.pData);

	// Copy the color data into the color constant buffer.
	colorDataPtr->apexColor = apexColor;
	colorDataPtr->centerColor = centerColor;

	// Unlock the color constant buffer.
	deviceContext->Unmap(m_colorBuffer.Get(), 0);

	// Set the position of the color constant buffer in the pixel shader.
	bufferNumber = 0;

	// Now set the color constant buffer in the pixel shader with the updated color values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_colorBuffer.GetAddressOf());

	return true;
}
