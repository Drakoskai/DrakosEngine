#include "pch.h"
#include "UserInterface.h"
#include "Utility.h"

UserInterface::UserInterface() :
m_Font1(nullptr),
m_FpsString(nullptr),
m_VideoStrings(nullptr),
m_PositionStrings(nullptr),
m_previousFps(0),
m_RenderCountStrings(nullptr),
m_MiniMap(nullptr)
{ }

UserInterface::UserInterface(const UserInterface&) : 
m_Font1(nullptr),
m_FpsString(nullptr),
m_VideoStrings(nullptr),
m_PositionStrings(nullptr),
m_previousFps(0), 
m_RenderCountStrings(nullptr),
m_MiniMap(nullptr)	{ }

UserInterface::~UserInterface()
{
	if (m_MiniMap)
	{
		delete m_MiniMap;
		m_MiniMap = nullptr;
	}

	if (m_RenderCountStrings)
	{
		delete[] m_RenderCountStrings;
		m_RenderCountStrings = nullptr;
	}

	if (m_PositionStrings)
	{
		delete[] m_PositionStrings;
		m_PositionStrings = nullptr;
	}

	if (m_VideoStrings)
	{
		delete[] m_VideoStrings;
		m_VideoStrings = nullptr;
	}

	if (m_FpsString)
	{
		delete m_FpsString;
		m_FpsString = nullptr;
	}

	if (m_Font1)
	{
		delete m_Font1;
		m_Font1 = nullptr;
	}
}

bool UserInterface::Initialize(DXDeviceResources* Direct3D, int screenHeight, int screenWidth)
{
	m_Font1 = new SimpleFont;

	if (!m_Font1->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "../Data/font01.txt", "../Data/font01.tga", 32.0f, 3))
	{
		return false;
	}

	m_FpsString = new Text;

	// Initialize the fps text string.
	if (!m_FpsString->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font1, "Fps: 0", 10, 50, 0.0f, 1.0f, 0.0f))
	{
		return false;
	}

	// Initial the previous frame fps.
	m_previousFps = -1;

	// Setup the video card strings.
	char videoCard[128];
	int videoMemory;

	Direct3D->GetVideoCardInfo(videoCard, videoMemory);
	char videoString[144];
	strcpy_s(videoString, "Video Card: ");
	strcat_s(videoString, videoCard);

	char tempString[16];
	tempString[0] = '\0';
	_itoa_s(videoMemory, tempString, 10);
	char memoryString[32];
	memoryString[0] = '\0';
	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	// Create the text objects for the video strings.
	m_VideoStrings = new Text[2];
	if (!m_VideoStrings)
	{
		return false;
	}

	// Initialize the video text strings.
	if (!m_VideoStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 256, false, m_Font1, videoString, 10, 10, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	if (!m_VideoStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_Font1, memoryString, 10, 30, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	// Create the text objects for the position strings.
	m_PositionStrings = new Text[6];
	if (!m_PositionStrings)
	{
		return false;
	}

	// Initialize the position text strings.
	if (!m_PositionStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font1, "X: 0", 10, 310, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	if (!m_PositionStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font1, "Y: 0", 10, 330, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	if (!m_PositionStrings[2].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font1, "Z: 0", 10, 350, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	if (!m_PositionStrings[3].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font1, "rX: 0", 10, 370, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	if (!m_PositionStrings[4].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font1, "rY: 0", 10, 390, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	if (!m_PositionStrings[5].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font1, "rZ: 0", 10, 410, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	// Initialize the previous frame position.
	for (int i = 0; i < 6; i++)
	{
		m_previousPosition[i] = -1;
	}

	// Create the text objects for the render count strings.
	m_RenderCountStrings = new Text[3];
	if (!m_RenderCountStrings)
	{
		return false;
	}

	// Initialize the render count strings.
	if (!m_RenderCountStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_Font1, "Polys Drawn: 0", 10, 260, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	if (!m_RenderCountStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_Font1, "Cells Drawn: 0", 10, 280, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	if (!m_RenderCountStrings[2].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_Font1, "Cells Culled: 0", 10, 300, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	// Create the mini-map object.
	m_MiniMap = new Minimap;
	if (!m_MiniMap)
	{
		return false;
	}

	// Initialize the mini-map object.
	if (!m_MiniMap->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 1025, 1025))
	{
		return false;
	}

	return true;
}

bool UserInterface::Frame(ID3D11DeviceContext* deviceContext, int fps, float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	// Update the fps string.
	if (!UpdateFpsString(deviceContext, fps))
	{
		return false;
	}

	// Update the position strings.
	if (!UpdatePositionStrings(deviceContext, posX, posY, posZ, rotX, rotY, rotZ))
	{
		return false;
	}

	// Update the mini-map position indicator.
	m_MiniMap->PositionUpdate(posX, posZ);

	return true;
}

bool UserInterface::Render(DXDeviceResources* Direct3D, ShaderManager* ShaderManager, Matrix worldMatrix, Matrix viewMatrix, Matrix orthoMatrix) const
{
	// Turn off the Z buffer and enable alpha blending to begin 2D rendering.
	Direct3D->TurnZBufferOff();
	Direct3D->EnableAlphaBlending();

	// Render the fps string.
	m_FpsString->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font1->GetTexture());

	// Render the video card strings.
	m_VideoStrings[0].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font1->GetTexture());
	m_VideoStrings[1].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font1->GetTexture());

	// Render the position and rotation strings.
	for (int i = 0; i < 6; i++)
	{
		m_PositionStrings[i].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font1->GetTexture());
	}

	// Render the render count strings.
	for (int i = 0; i < 3; i++)
	{
		m_RenderCountStrings[i].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font1->GetTexture());
	}

	// Turn off alpha blending now that the text has been rendered.
	Direct3D->DisableAlphaBlending();

	// Render the mini-map.
	if (!m_MiniMap->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix))
	{
		return false;
	}

	// Turn the Z buffer back on now that the 2D rendering has completed.
	Direct3D->TurnZBufferOn();

	return true;
}

bool UserInterface::UpdateFpsString(ID3D11DeviceContext* deviceContext, int fps)
{
	// Check if the fps from the previous frame was the same, if so don't need to update the text string.
	if (m_previousFps == fps)
	{
		return true;
	}

	// Store the fps for checking next frame.
	m_previousFps = fps;

	// Truncate the fps to below 100,000.
	if (fps > 99999)
	{
		fps = 99999;
	}
	char tempString[16];
	char finalString[16];
	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(finalString, "Fps: ");
	strcat_s(finalString, tempString);

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	return m_FpsString->UpdateSentence(deviceContext, m_Font1, finalString, 10, 50, red, green, blue);
}

bool UserInterface::UpdatePositionStrings(ID3D11DeviceContext* deviceContext, float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	int positionX;
	int positionY;
	int positionZ;
	int rotationX;
	int rotationY;
	int rotationZ;
	char tempString[16];
	char finalString[16];
	
	// Convert the float values to integers.
	positionX = static_cast<int>(posX);
	positionY = static_cast<int>(posY);
	positionZ = static_cast<int>(posZ);
	rotationX = static_cast<int>(rotX);
	rotationY = static_cast<int>(rotY);
	rotationZ = static_cast<int>(rotZ);

	// Update the position strings if the value has changed since the last frame.
	if (positionX != m_previousPosition[0])
	{
		m_previousPosition[0] = positionX;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "X: ");
		strcat_s(finalString, tempString);
		bool result = m_PositionStrings[0].UpdateSentence(deviceContext, m_Font1, finalString, 10, 100, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (positionY != m_previousPosition[1])
	{
		m_previousPosition[1] = positionY;
		_itoa_s(positionY, tempString, 10);
		strcpy_s(finalString, "Y: ");
		strcat_s(finalString, tempString);
		bool result = m_PositionStrings[1].UpdateSentence(deviceContext, m_Font1, finalString, 10, 120, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (positionZ != m_previousPosition[2])
	{
		m_previousPosition[2] = positionZ;
		_itoa_s(positionZ, tempString, 10);
		strcpy_s(finalString, "Z: ");
		strcat_s(finalString, tempString);
		bool result = m_PositionStrings[2].UpdateSentence(deviceContext, m_Font1, finalString, 10, 140, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (rotationX != m_previousPosition[3])
	{
		m_previousPosition[3] = rotationX;
		_itoa_s(rotationX, tempString, 10);
		strcpy_s(finalString, "rX: ");
		strcat_s(finalString, tempString);
		bool result = m_PositionStrings[3].UpdateSentence(deviceContext, m_Font1, finalString, 10, 180, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (rotationY != m_previousPosition[4])
	{
		m_previousPosition[4] = rotationY;
		_itoa_s(rotationY, tempString, 10);
		strcpy_s(finalString, "rY: ");
		strcat_s(finalString, tempString);
		bool result = m_PositionStrings[4].UpdateSentence(deviceContext, m_Font1, finalString, 10, 200, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (rotationZ != m_previousPosition[5])
	{
		m_previousPosition[5] = rotationZ;
		_itoa_s(rotationZ, tempString, 10);
		strcpy_s(finalString, "rZ: ");
		strcat_s(finalString, tempString);
		bool result = m_PositionStrings[5].UpdateSentence(deviceContext, m_Font1, finalString, 10, 220, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	return true;
}

bool UserInterface::UpdateRenderCounts(ID3D11DeviceContext* deviceContext, int renderCount, int nodesDrawn, int nodesCulled) const
{
	char tempString[32];
	char finalString[32];

	// Convert the render count integer to string format.
	_itoa_s(renderCount, tempString, 10);

	// Setup the render count string.
	strcpy_s(finalString, "Polys Drawn: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	if (!m_RenderCountStrings[0].UpdateSentence(deviceContext, m_Font1, finalString, 10, 260, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	// Convert the cells drawn integer to string format.
	_itoa_s(nodesDrawn, tempString, 10);

	// Setup the cells drawn string.
	strcpy_s(finalString, "Cells Drawn: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	if (!m_RenderCountStrings[1].UpdateSentence(deviceContext, m_Font1, finalString, 10, 280, 1.0f, 1.0f, 1.0f))
	{
		return false;
	}

	// Convert the cells culled integer to string format.
	_itoa_s(nodesCulled, tempString, 10);

	// Setup the cells culled string.
	strcpy_s(finalString, "Cells Culled: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	return m_RenderCountStrings[2].UpdateSentence(deviceContext, m_Font1, finalString, 10, 300, 1.0f, 1.0f, 1.0f);
}
