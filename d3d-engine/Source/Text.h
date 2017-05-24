#pragma once

#include <d3d11_2.h>
#include "Font.h"
#include "ShaderManager.h"

class Text {
	struct VertexType {
		Vector3 position;
		Vector2 texture;
	};

public:
	Text();
	~Text();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, int, bool, SimpleFont*, char*, int, int, float, float, float);
	void Render(ID3D11DeviceContext*, ShaderManager*, Matrix, Matrix, Matrix, ID3D11ShaderResourceView*) const;
	bool UpdateSentence(ID3D11DeviceContext*, SimpleFont*, char*, int, int, float, float, float);

private:
	Text(const Text&);

	bool InitializeSentence(ID3D11Device*, ID3D11DeviceContext*, SimpleFont*, char*, int, int, float, float, float);
	void RenderSentence(ID3D11DeviceContext*, ShaderManager*, Matrix, Matrix, Matrix, ID3D11ShaderResourceView*) const;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_shadowVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_shadowIndexBuffer;

	int m_screenWidth;
	int m_screenHeight;

	int m_maxLength;
	int m_vertexCount;
	int m_indexCount;
	bool m_shadow;

	Color m_pixelColor;
};
