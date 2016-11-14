#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1500.0f;
const float SCREEN_NEAR = 0.1f;

#include "InputContext.h"
#include "DXDeviceResources.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "GameTimer.h"
#include "Fps.h"
#include "Scene.h"

class Game
{
public:
	Game();
	~Game();

	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	bool Frame(InputContext* input) const;

private:
	Game(const Game&);

	DXDeviceResources* m_Direct3D;
	ShaderManager* m_ShaderManager;
	TextureManager* m_TextureManager;
	GameTimer* m_Timer;
	Fps* m_Fps;
	Scene* mScene;

};
