#include "pch.h"
#include "DXPipelineState.h"
#include "DXHelper.h"

DXPipelineState::DXPipelineState() { }

DXPipelineState::~DXPipelineState() { }

void DXPipelineState::InitAll(ID3D11Device* device)
{
	InitRasterState(device);

	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));

	noCullDesc.AntialiasedLineEnable = false;
	noCullDesc.CullMode = D3D11_CULL_BACK;
	noCullDesc.DepthBias = 0;
	noCullDesc.DepthBiasClamp = 0.0f;
	noCullDesc.DepthClipEnable = true;
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.MultisampleEnable = false;
	noCullDesc.ScissorEnable = false;
	noCullDesc.SlopeScaledDepthBias = 0.0f;
	noCullDesc.CullMode = D3D11_CULL_NONE;

	HRESULT result = device->CreateRasterizerState(&noCullDesc, m_rasterStateNoCulling.GetAddressOf());
	DX::ThrowIfFailed(result);

	// Setup a raster description which enables wire frame rendering.
	D3D11_RASTERIZER_DESC wireFrameDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));

	wireFrameDesc.AntialiasedLineEnable = false;
	wireFrameDesc.CullMode = D3D11_CULL_BACK;
	wireFrameDesc.DepthBias = 0;
	wireFrameDesc.DepthBiasClamp = 0.0f;
	wireFrameDesc.DepthClipEnable = true;
	wireFrameDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireFrameDesc.FrontCounterClockwise = false;
	wireFrameDesc.MultisampleEnable = false;
	wireFrameDesc.ScissorEnable = false;
	wireFrameDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&wireFrameDesc, m_rasterStateWireframe.GetAddressOf());
	DX::ThrowIfFailed(result);

	InitAlphaBlendState(device);

	// create an alpha disabled blend state description.
	D3D11_BLEND_DESC blendDisabledStateDescription;
	ZeroMemory(&blendDisabledStateDescription, sizeof(D3D11_BLEND_DESC));

	blendDisabledStateDescription.AlphaToCoverageEnable = FALSE;
	blendDisabledStateDescription.IndependentBlendEnable = false;
	blendDisabledStateDescription.RenderTarget[0].BlendEnable = true;
	blendDisabledStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDisabledStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDisabledStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDisabledStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDisabledStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDisabledStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDisabledStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	blendDisabledStateDescription.RenderTarget[0].BlendEnable = false;
	blendDisabledStateDescription.AlphaToCoverageEnable = false;

	result = device->CreateBlendState(&blendDisabledStateDescription, m_alphaDisableBlendState.GetAddressOf());
	DX::ThrowIfFailed(result);

	// Create a blend state description for the alpha-to-coverage blending mode.
	D3D11_BLEND_DESC alphaCoverageBlendStateDesc;
	ZeroMemory(&alphaCoverageBlendStateDesc, sizeof(D3D11_BLEND_DESC));

	alphaCoverageBlendStateDesc.AlphaToCoverageEnable = true;
	alphaCoverageBlendStateDesc.IndependentBlendEnable = false;
	alphaCoverageBlendStateDesc.RenderTarget[0].BlendEnable = true;
	alphaCoverageBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaCoverageBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaCoverageBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	alphaCoverageBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaCoverageBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaCoverageBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	alphaCoverageBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = device->CreateBlendState(&alphaCoverageBlendStateDesc, m_alphaCoverageBlendState.GetAddressOf());
	DX::ThrowIfFailed(result);

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
	DX::ThrowIfFailed(result);

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthDisabledStencilDesc, m_depthDisabledStencilState.GetAddressOf());
	DX::ThrowIfFailed(result);
}

void DXPipelineState::InitRasterState(ID3D11Device* device)
{
	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	HRESULT result = device->CreateRasterizerState(&rasterDesc, m_rasterState.GetAddressOf());
	DX::ThrowIfFailed(result);
}

void DXPipelineState::InitAlphaBlendState(ID3D11Device* device)
{
	// Create an alpha enabled blend state description.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	blendStateDescription.AlphaToCoverageEnable = FALSE;
	blendStateDescription.IndependentBlendEnable = false;
	blendStateDescription.RenderTarget[0].BlendEnable = true;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	HRESULT result = device->CreateBlendState(&blendStateDescription, mAplhaBlend.GetAddressOf());
	DX::ThrowIfFailed(result);
}
