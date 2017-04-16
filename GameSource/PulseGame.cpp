#include "PulseGame.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"


void PulseGame::InitTestScene()
{	
	ThisScene->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/Irradiance/Irradiance.dds"));
	ThisScene->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/dayCube.dds"));

	auto dirL = ThisScene->GetLight()->GetModDirectionalLight();

	dirL->LightColor[0] = 1.0f;
	dirL->LightColor[1] = 0.76f;
	dirL->LightColor[2] = 0.21f;

	dirL->LightIntensity[0] = 3.2f;
	dirL->LightIntensity[1] = 0.1f;

	dirL->Direction[0] = 0.1f;
	dirL->Direction[1] = -0.6f;
	dirL->Direction[2] = -0.8f;

	ThisScene->GetCamera()->SetPosition(0.0f, 0.0f, -10.0f);

	m_lineData = LoadLine(Manager->GetFileManager()->LoadFile("Resource/testCurve.pld"));

	auto smoothLine = MathHelper::CatmullromSpline(m_lineData, 20, false);
	auto tangents = MathHelper::CatmullromSpline(m_lineData, 20, true);

	auto obj = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);
	obj->GetRenderer()->Model = Manager->GetModel()->AddTubeFromLineData(smoothLine, tangents, 0.1f, smoothLine);
	obj->SetName("Tube");

	m_player.Initialize(Manager, m_lineData);
}

void PulseGame::UpdateTestScene(float dt)
{
	if (GetAsyncKeyState('P') & 0x8000)
	{
		m_pause = true;
	}
	
	if (GetAsyncKeyState('U') & 0x8000)
	{
		m_pause = false;
	}

	if(!m_pause)
		m_player.UpdateShipPos(dt);
}

void PulseGame::Init()
{
	InitTestScene();
}

void PulseGame::Update(float dt)
{
	UpdateTestScene(dt);
}
