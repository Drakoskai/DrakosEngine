#pragma once

#include <d3d11_2.h>

class DXPipelineState {
public:
	DXPipelineState();
	~DXPipelineState();

	void InitAll(ID3D11Device* device);

	//Rasterizer States
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> CullCounterClockWise;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> NoCull;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> WireFrame;

	// Blend States
	Microsoft::WRL::ComPtr<ID3D11BlendState> AlphaBlend;
	Microsoft::WRL::ComPtr<ID3D11BlendState> AlphaBlendDisabled;
	Microsoft::WRL::ComPtr<ID3D11BlendState> NonPremultiplied;

	// Depth/stencil states
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthDisabledStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

private:
	static HRESULT CreateRasterizerState(ID3D11Device* device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, _Out_ ID3D11RasterizerState** pResult);

	static HRESULT CreateBlendState(ID3D11Device* device, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, _Out_ ID3D11BlendState** pResult);

	static HRESULT DXPipelineState::CreateBlendState(ID3D11Device* device,
		D3D11_BLEND srcBlend,
		D3D11_BLEND srcBlendAlpha,
		D3D11_BLEND destBlend,
		D3D11_BLEND DestBlendAlpha,
		UINT8 writeMask,
		_Out_ ID3D11BlendState** pResult);

	static HRESULT CreateBlendState(ID3D11Device* device,
		D3D11_BLEND srcBlend,
		D3D11_BLEND srcBlendAlpha,
		D3D11_BLEND destBlend,
		D3D11_BLEND DestBlendAlpha,
		UINT8 writeMask,
		bool blendEnable,
		_Out_ ID3D11BlendState** pResult);

};
