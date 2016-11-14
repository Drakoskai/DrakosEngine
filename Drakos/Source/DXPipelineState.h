#pragma once

#include <d3d11_2.h>

class DXPipelineState
{
public:
	DXPipelineState();
	~DXPipelineState();

	void InitAll(ID3D11Device* device);

	//Rasterizer States
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterStateNoCulling;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterStateWireframe;

	// Blend States
	Microsoft::WRL::ComPtr<ID3D11BlendState> mAplhaBlend;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaDisableBlendState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaCoverageBlendState;

	// Depth/stencil states
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthDisabledStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

private:
	void InitRasterState(ID3D11Device* device);
	void InitAlphaBlendState(ID3D11Device* device);
};
