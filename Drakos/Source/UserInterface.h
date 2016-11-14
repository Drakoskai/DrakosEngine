#pragma once

#include "Text.h"
#include "Minimap.h"
#include "DXDeviceResources.h"
#include "DXMath.h"

class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	bool Initialize(DXDeviceResources*, int, int);
	bool Frame(ID3D11DeviceContext*, int, float, float, float, float, float, float);
	bool Render(DXDeviceResources*, ShaderManager*, Matrix, Matrix, Matrix) const;
	bool UpdateRenderCounts(ID3D11DeviceContext*, int, int, int) const;

private:
	UserInterface(const UserInterface&);

	bool UpdateFpsString(ID3D11DeviceContext*, int);
	bool UpdatePositionStrings(ID3D11DeviceContext*, float, float, float, float, float, float);

	SimpleFont* m_Font1;
	Text *m_FpsString;
	Text *m_VideoStrings;
	Text *m_PositionStrings;
	int m_previousFps;
	int m_previousPosition[6];
	Text* m_RenderCountStrings;
	Minimap* m_MiniMap;
	
};
