#include "pch.h"
#include "Scene.h"

Scene::Scene() :
m_UserInterface(nullptr),
m_Camera(nullptr),
m_Light(nullptr),
m_CameraController(nullptr),
m_Frustum(nullptr),
m_SkyDome(nullptr),
m_Terrain(nullptr),
m_displayUI(false),
m_wireFrame(false),
m_cellLines(false),
m_heightLocked(false) { }

Scene::Scene(const Scene&) :
m_UserInterface(nullptr),
m_Camera(nullptr),
m_Light(nullptr),
m_CameraController(nullptr),
m_Frustum(nullptr),
m_SkyDome(nullptr),
m_Terrain(nullptr),
m_displayUI(false),
m_wireFrame(false),
m_cellLines(false),
m_heightLocked(false) { }

Scene::~Scene()
{
	if (m_Terrain)
	{
		delete m_Terrain;
		m_Terrain = nullptr;
	}

	if (m_SkyDome)
	{
		delete m_SkyDome;
		m_SkyDome = nullptr;
	}

	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = nullptr;
	}

	if (m_CameraController)
	{
		delete m_CameraController;
		m_CameraController = nullptr;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = nullptr;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	if (m_UserInterface)
	{
		delete m_UserInterface;
		m_UserInterface = nullptr;
	}
}

bool Scene::Initialize(DXDeviceResources* direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth)
{
	m_UserInterface = new UserInterface;
	if (!m_UserInterface->Initialize(direct3D, screenHeight, screenWidth))
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	m_Camera = new SimpleCamera;
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();

	m_Light = new Light;
	m_Light->SetDiffuseColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light->SetDirection(Vector3(-0.5f, -1.0f, -0.5f));

	m_CameraController = new CameraController;
	m_CameraController->SetPosition(512.5f, 10.0f, 10.0f);
	m_CameraController->SetRotation(0.0f, 0.0f, 0.0f);

	m_Frustum = new Frustum;
	m_Frustum->Initialize(screenDepth);

	m_SkyDome = new SkyDomeModel;
	if (!m_SkyDome->Initialize(direct3D->GetDevice()))
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	m_Terrain = new Terrain;
	if (!m_Terrain->Initialize(direct3D->GetDevice(), "../Data/setup.txt"))
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	m_displayUI = true;
	m_wireFrame = false;
	m_cellLines = false;
	m_heightLocked = true;

	return true;
}

bool Scene::Frame(DXDeviceResources* Direct3D, InputContext* Input, ShaderManager* ShaderManager, TextureManager* TextureManager, float frameTime, int fps)
{
	bool foundHeight;
	float posX;
	float posY;
	float posZ;
	float rotX;
	float rotY;
	float rotZ;
	float height;

	// Do the frame input processing.
	HandleMovementInput(Input, frameTime);

	// Get the view point position/rotation.
	m_CameraController->GetPosition(posX, posY, posZ);
	m_CameraController->GetRotation(rotX, rotY, rotZ);

	// Do the frame processing for the user interface.
	if (!m_UserInterface->Frame(Direct3D->GetDeviceContext(), fps, posX, posY, posZ, rotX, rotY, rotZ))
	{
		return false;
	}

	// Do the terrain frame processing.
	m_Terrain->Frame();

	// If the height is locked to the terrain then position the camera on top of it.
	if (m_heightLocked)
	{
		// Get the height of the triangle that is directly underneath the given camera position.
		foundHeight = m_Terrain->GetHeightAtPosition(posX, posZ, height);
		if (foundHeight)
		{
			// If there was a triangle under the camera then position the camera just above it by one meter.
			m_CameraController->SetPosition(posX, height + 1.0f, posZ);
			m_Camera->SetPosition(posX, height + 1.0f, posZ);
		}
	}

	// Render the graphics.
	return Render(Direct3D, ShaderManager, TextureManager);
}

void Scene::HandleMovementInput(InputContext* gameInput, float frameTime)
{
	bool keyDown;
	float posX;
	float posY;
	float posZ;
	float rotX;
	float rotY;
	float rotZ;

	// Set the frame time for calculating the updated position.
	m_CameraController->SetFrameTime(frameTime);

	// Handle the input.
	KeyEvent keyEvent;
	gameInput->GetKeyEvent(keyEvent);
	m_CameraController->KeyPressed(keyEvent);

	keyDown = gameInput->IsAPressed();
	m_CameraController->MoveUpward(keyDown);

	keyDown = gameInput->IsZPressed();
	m_CameraController->MoveDownward(keyDown);

	keyDown = gameInput->IsPgUpPressed();
	m_CameraController->LookUpward(keyDown);

	keyDown = gameInput->IsPgDownPressed();
	m_CameraController->LookDownward(keyDown);

	// Get the view point position/rotation.
	m_CameraController->GetPosition(posX, posY, posZ);
	m_CameraController->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Determine if the user interface should be displayed or not.
	if (gameInput->IsF1Toggled())
	{
		m_displayUI = !m_displayUI;
	}

	// Determine if the terrain should be rendered in wireframe or not.
	if (gameInput->IsF2Toggled())
	{
		m_wireFrame = !m_wireFrame;
	}

	// Determine if we should render the lines around each terrain cell.
	if (gameInput->IsF3Toggled())
	{
		m_cellLines = !m_cellLines;
	}

	// Determine if we should be locked to the terrain height when we move around or not.
	if (gameInput->IsF4Toggled())
	{
		m_heightLocked = !m_heightLocked;
	}
}

bool Scene::Render(DXDeviceResources* direct3D, ShaderManager* shaderManager, TextureManager* textureManager) const
{
	Matrix worldMatrix;
	Matrix projectionMatrix;
	Matrix orthoMatrix;
	Vector3 cameraPosition;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	direct3D->GetWorldMatrix(worldMatrix);
	Matrix viewMatrix = m_Camera->GetProjMatrix();
	direct3D->GetProjMatrix(projectionMatrix);
	Matrix baseViewMatrix = m_Camera->GetViewMatrix();
	direct3D->GetOrthoMatrix(orthoMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Construct the frustum.
	m_Frustum->ConstructFrustum(projectionMatrix, viewMatrix);

	// Clear the buffers to begin the scene.
	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Turn off back face culling and turn off the Z buffer.
	direct3D->TurnOffCulling();
	direct3D->TurnZBufferOff();

	// Translate the sky dome to be centered around the camera position.
	worldMatrix = DirectX::XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(direct3D->GetDeviceContext());
	if (!shaderManager->RenderSkyDomeShader(direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor()))
	{
		return false;
	}

	// Reset the world matrix.
	direct3D->GetWorldMatrix(worldMatrix);

	// Turn the Z buffer back and back face culling on.
	direct3D->TurnZBufferOn();
	direct3D->TurnOnCulling();

	// Turn on wire frame rendering of the terrain if needed.
	if (m_wireFrame)
	{
		direct3D->EnableWireframe();
	}

	// Render the terrain cells (and cell lines if needed).
	for (int i = 0; i < m_Terrain->GetCellCount(); i++)
	{
		// Render each terrain cell if it is visible only.
		if (m_Terrain->RenderCell(direct3D->GetDeviceContext(), i, m_Frustum))
		{
			// Render the cell buffers using the hgih quality terrain shader.
			if (!shaderManager->RenderTerrainShader(direct3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i),
				worldMatrix, viewMatrix, projectionMatrix, textureManager->GetTexture(0), textureManager->GetTexture(1), 
				textureManager->GetTexture(2), textureManager->GetTexture(3), m_Light->GetDirection(), m_Light->GetDiffuseColor()))
			{
				return false;
			}

			// If needed then render the bounding box around this terrain cell using the color shader. 
			if (m_cellLines)
			{
				m_Terrain->RenderCellLines(direct3D->GetDeviceContext(), i);
				if (!shaderManager->RenderColorShader(direct3D->GetDeviceContext(), m_Terrain->GetCellLinesIndexCount(i),
					worldMatrix, viewMatrix, projectionMatrix))
				{
					return false;
				}
			}
		}
	}

	// Turn off wire frame rendering of the terrain if it was on.
	if (m_wireFrame)
	{
		direct3D->DisableWireframe();
	}

	// Update the render counts in the UI.
	if (!m_UserInterface->UpdateRenderCounts(direct3D->GetDeviceContext(), m_Terrain->GetRenderCount(), m_Terrain->GetCellsDrawn(), m_Terrain->GetCellsCulled()))
	{
		return false;
	}

	// Render the user interface.
	if (m_displayUI)
	{
		if (!m_UserInterface->Render(direct3D, shaderManager, worldMatrix, baseViewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	direct3D->EndScene();

	return true;
}
