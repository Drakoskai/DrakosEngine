#include "pch.h"
#include "DXPipelineState.h"
#include "DXHelper.h"

DXPipelineState::DXPipelineState() {}

DXPipelineState::~DXPipelineState() {}

void DXPipelineState::InitAll(ID3D11Device* device) {
	HRESULT result;

	CreateRasterizerState(device, D3D11_CULL_BACK, D3D11_FILL_SOLID, CullCounterClockWise.GetAddressOf());
	CreateRasterizerState(device, D3D11_CULL_NONE, D3D11_FILL_SOLID, NoCull.GetAddressOf());
	CreateRasterizerState(device, D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, WireFrame.GetAddressOf());

	CreateBlendState(device, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, 0x0f, AlphaBlend.GetAddressOf());
	CreateBlendState(device, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, 0x0f, false, AlphaBlendDisabled.GetAddressOf());
	CreateBlendState(device, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, NonPremultiplied.GetAddressOf());

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

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

HRESULT DXPipelineState::CreateRasterizerState(ID3D11Device* device,
	D3D11_CULL_MODE cullMode,
	D3D11_FILL_MODE fillMode,
	_Out_ ID3D11RasterizerState** pResult) {
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = cullMode;
	desc.FillMode = fillMode;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;
	HRESULT hr = device->CreateRasterizerState(&desc, pResult);

	DX::ThrowIfFailed(hr);

	return hr;
}

HRESULT DXPipelineState::CreateBlendState(ID3D11Device* device,	D3D11_BLEND srcBlend,
	D3D11_BLEND srcBlendAlpha,
	D3D11_BLEND destBlend,
	D3D11_BLEND DestBlendAlpha,
	UINT8 writeMask,
	_Out_ ID3D11BlendState** pResult) {
	D3D11_BLEND_DESC desc = {};

	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);

	desc.RenderTarget[0].SrcBlend = srcBlend;
	desc.RenderTarget[0].SrcBlendAlpha = srcBlendAlpha;
	desc.RenderTarget[0].DestBlend = destBlend;
	desc.RenderTarget[0].DestBlendAlpha = DestBlendAlpha;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = writeMask;

	HRESULT hr = device->CreateBlendState(&desc, pResult);

	DX::ThrowIfFailed(hr);

	return hr;
}

HRESULT DXPipelineState::CreateBlendState(ID3D11Device* device,
	D3D11_BLEND srcBlend,
	D3D11_BLEND srcBlendAlpha,
	D3D11_BLEND destBlend,
	D3D11_BLEND DestBlendAlpha,
	UINT8 writeMask,
	bool blendEnable,
	_Out_ ID3D11BlendState** pResult) {
	D3D11_BLEND_DESC desc = {};

	desc.RenderTarget[0].BlendEnable = blendEnable;

	desc.RenderTarget[0].SrcBlend = srcBlend;
	desc.RenderTarget[0].SrcBlendAlpha = srcBlendAlpha;
	desc.RenderTarget[0].DestBlend = destBlend;
	desc.RenderTarget[0].DestBlendAlpha = DestBlendAlpha;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = writeMask;

	HRESULT hr = device->CreateBlendState(&desc, pResult);

	DX::ThrowIfFailed(hr);

	return hr;
}

HRESULT DXPipelineState::CreateBlendState(ID3D11Device* device,
	D3D11_BLEND srcBlend,
	D3D11_BLEND destBlend,
	_Out_ ID3D11BlendState** pResult) {
	D3D11_BLEND_DESC desc = {};

	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);

	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = device->CreateBlendState(&desc, pResult);

	DX::ThrowIfFailed(hr);

	return hr;
}