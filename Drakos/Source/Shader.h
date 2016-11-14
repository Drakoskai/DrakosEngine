#pragma once
#include <winnt.h>
#include <DirectXMath.h>

__interface IShader
{
public:

	void SetUniform(WCHAR* name, int value);

	void SetUniform(WCHAR* name, float value);

	void SetUniform(WCHAR* name, DirectX::XMFLOAT2 value);

	void SetUniform(WCHAR* name, DirectX::XMFLOAT3 value);

	void SetUniform(WCHAR* name, DirectX::XMFLOAT4 value) ;

	void SetUniform(WCHAR* name, DirectX::XMMATRIX value) ;
};
