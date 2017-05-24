#pragma once

#include <windows.h>
#include "DXDeviceResources.h"
#include "InputContext.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "UserInterface.h"
#include "Camera.h"
#include "Light.h"
#include "CameraController.h"
#include "Frustum.h"
#include "Skydome.h"
#include "Terrain.h"

class Scene {
public:
	Scene();
	~Scene();

	bool Initialize(DXDeviceResources* direct3D, HWND hwnd, int width, int height, float depth);
	bool Frame(DXDeviceResources* direct3D, InputContext* Input, ShaderManager* shaderManager, TextureManager* textureManager, float frameTime, int fps);

private:
	Scene(const Scene&);
	void HandleMovementInput(InputContext* input, float frameTime);
	bool Render(DXDeviceResources* direct3D, ShaderManager* shaderManager, TextureManager* textureManager) const;

	UserInterface* m_UserInterface;
	SimpleCamera* m_Camera;
	Light* m_Light;
	CameraController* m_CameraController;
	Frustum* m_Frustum;
	SkyDomeModel* m_SkyDome;
	Terrain* m_Terrain;

	bool m_displayUI;
	bool m_wireFrame;
	bool m_cellLines;
	bool m_heightLocked;

};
