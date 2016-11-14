#include "pch.h"
#include "DXDeviceResources.h"
#include "DXPipelineState.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

DXDeviceResources::DXDeviceResources() :
m_vsync_enabled(false),
m_videoCardMemory(0),
m_swapChain(nullptr),
m_device(nullptr),
m_deviceContext(nullptr),
m_renderTargetView(nullptr),
m_depthStencilBuffer(nullptr),
m_depthStencilView(nullptr),
m_pipeline(nullptr) { }

DXDeviceResources::DXDeviceResources(const DXDeviceResources&) :
m_vsync_enabled(false),
m_videoCardMemory(0),
m_swapChain(nullptr),
m_device(nullptr),
m_deviceContext(nullptr),
m_renderTargetView(nullptr),
m_depthStencilBuffer(nullptr),
m_depthStencilView(nullptr),
m_pipeline(nullptr) { }

DXDeviceResources::~DXDeviceResources()
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, nullptr);
	}
	if (m_pipeline)
	{
		delete m_pipeline;
		m_pipeline = nullptr;
	}
}

bool DXDeviceResources::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	m_vsync_enabled = vsync;

	ComPtr<IDXGIFactory> factory;
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}
	ComPtr<IDXGIAdapter> adapter;
	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, adapter.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	ComPtr<IDXGIOutput> adapterOutput;
	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	unsigned int numModes(0);
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	DXGI_MODE_DESC* displayModeList;

	displayModeList = new DXGI_MODE_DESC[numModes];
	// fill the display mode list
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	unsigned int numerator = 0;
	unsigned int denominator = 0;
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == static_cast<unsigned int>(screenWidth))
		{
			if (displayModeList[i].Height == static_cast<unsigned int>(screenHeight))
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
	ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	unsigned long long stringLength;
	int error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;

	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, m_swapChain.GetAddressOf(), m_device.GetAddressOf(), nullptr, m_deviceContext.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	ID3D11Texture2D* backBufferPtr = nullptr;
	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, nullptr, m_renderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	if (FAILED(m_device->CreateTexture2D(&depthBufferDesc, nullptr, m_depthStencilBuffer.GetAddressOf())))
	{
		return false;
	}

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// Create the depth stencil view.
	if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf())))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	m_pipeline = new DXPipelineState();
	// Now set the rasterizer state.
	m_pipeline->InitAll(m_device.Get());
	m_deviceContext->RSSetState(m_pipeline->m_rasterState.Get());

	// Setup the viewport for rendering.
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), screenNear, screenDepth);

	return true;
}

void DXDeviceResources::BeginScene(float red, float green, float blue, float alpha) const
{
	float color[4];
	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DXDeviceResources::EndScene() const
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device* DXDeviceResources::GetDevice() const
{
	return m_device.Get();
}

ID3D11DeviceContext* DXDeviceResources::GetDeviceContext() const
{
	return m_deviceContext.Get();
}

void DXDeviceResources::GetProjMatrix(Matrix& projMatrix) const
{
	projMatrix = m_projectionMatrix;
}

void DXDeviceResources::GetWorldMatrix(Matrix& worldMatrix) const
{
	worldMatrix = m_worldMatrix;
}

void DXDeviceResources::GetOrthoMatrix(Matrix& orthoMatrix) const
{
	orthoMatrix = m_orthoMatrix;
}

void DXDeviceResources::GetVideoCardInfo(char* cardName, int& memory) const
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}

void DXDeviceResources::TurnZBufferOn() const
{
	m_deviceContext->OMSetDepthStencilState(m_pipeline->m_depthStencilState.Get(), 1);
}

void DXDeviceResources::TurnZBufferOff() const
{
	m_deviceContext->OMSetDepthStencilState(m_pipeline->m_depthDisabledStencilState.Get(), 1);
}

void DXDeviceResources::EnableAlphaBlending() const
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_pipeline->mAplhaBlend.Get(), blendFactor, 0xffffffff);
}

void DXDeviceResources::DisableAlphaBlending() const
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_deviceContext->OMSetBlendState(m_pipeline->m_alphaDisableBlendState.Get(), blendFactor, 0xffffffff);
}

void DXDeviceResources::TurnOnCulling() const
{
	// Set the culling rasterizer state.
	m_deviceContext->RSSetState(m_pipeline->m_rasterState.Get());
}

void DXDeviceResources::TurnOffCulling() const
{
	// Set the no back face culling rasterizer state.
	m_deviceContext->RSSetState(m_pipeline->m_rasterStateNoCulling.Get());
}

void DXDeviceResources::EnableAlphaToCoverageBlending() const
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_pipeline->m_alphaCoverageBlendState.Get(), blendFactor, 0xffffffff);
}

void DXDeviceResources::EnableWireframe() const
{
	// Set the wire frame rasterizer state.
	m_deviceContext->RSSetState(m_pipeline->m_rasterStateWireframe.Get());
}

void DXDeviceResources::DisableWireframe() const
{
	// Set the solid fill rasterizer state.
	m_deviceContext->RSSetState(m_pipeline->m_rasterState.Get());
}
