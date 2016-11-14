#pragma once

#include <d3d11_2.h>
#include <wrl.h>
#include "DXMath.h"
#include "DXPipelineState.h"

class DXDeviceResources
{
public:
	

	DXDeviceResources();
	DXDeviceResources(const DXDeviceResources& other);
	~DXDeviceResources();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void BeginScene(float red, float green, float blue, float alpha) const;
	void EndScene() const;
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	void GetProjMatrix(Matrix& projMatrix) const;
	void GetWorldMatrix(Matrix& worldMatrix) const;
	void GetOrthoMatrix(Matrix& orthoMatrix) const;
	void GetVideoCardInfo(char* cardName, int& memory) const;
	void TurnZBufferOn() const;
	void TurnZBufferOff() const;
	void TurnOnCulling() const;
	void TurnOffCulling() const;
	void EnableAlphaBlending() const;
	void EnableAlphaToCoverageBlending() const;
	void DisableAlphaBlending() const;
	void EnableWireframe() const;
	void DisableWireframe() const;

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	DXPipelineState * m_pipeline;
	Matrix m_projectionMatrix;
	Matrix m_worldMatrix;
	Matrix m_orthoMatrix;

};
