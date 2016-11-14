#include "pch.h"
#include "Game.h"
#include "Utility.h"

Game::Game() :
m_Direct3D(nullptr),
m_ShaderManager(nullptr),
m_TextureManager(nullptr),
m_Timer(nullptr),
m_Fps(nullptr),
mScene(nullptr) { }


Game::Game(const Game&) :
m_Direct3D(nullptr),
m_ShaderManager(nullptr),
m_TextureManager(nullptr),
m_Timer(nullptr),
m_Fps(nullptr),
mScene(nullptr) { }

Game::~Game()
{
	if (mScene)
	{
		delete mScene;
		mScene = nullptr;
	}

	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = nullptr;
	}
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = nullptr;
	}
	if (m_TextureManager)
	{
		delete m_TextureManager;
		m_TextureManager = nullptr;
	}

	if (m_ShaderManager)
	{
		delete m_ShaderManager;
		m_ShaderManager = nullptr;
	}

	if (m_Direct3D)
	{
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}
}

bool Game::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	UNREFERENCED_PARAMETER(hinstance);

	m_Direct3D = new DXDeviceResources;
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	m_ShaderManager = new ShaderManager;
	if (!m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	m_TextureManager = new TextureManager;
	if (!m_TextureManager->Initialize(10))
	{
		MessageBox(hwnd, L"Could not initialize the texture manager object.", L"Error", MB_OK);
		return false;
	}

	if (!m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../Data/rock01d.tga", 0))
	{
		return false;
	}

	if (!m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../Data/rock01n.tga", 1))
	{
		return false;
	}

	if (!m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../Data/snow01n.tga", 2))
	{
		return false;
	}

	if (!m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../Data/distance01n.tga", 3))
	{
		return false;
	}

	m_Timer = new GameTimer;
	if (!m_Timer->Initialize())
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	m_Fps = new Fps;
	m_Fps->Initialize();

	mScene = new Scene;
	if (!mScene->Initialize(m_Direct3D, hwnd, screenWidth, screenHeight, SCREEN_DEPTH))
	{
		MessageBox(hwnd, L"Could not initialize the zone object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool Game::Frame(InputContext* input) const
{
	m_Fps->Frame();
	m_Timer->Frame();

	if (!mScene->Frame(m_Direct3D, input, m_ShaderManager, m_TextureManager, m_Timer->GetTime(), m_Fps->GetFps()))
	{
		return false;
	}

	return true;
}
