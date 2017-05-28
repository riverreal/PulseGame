#include "PulseGame.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/ENote.h"

void PulseGame::InitTestScene()
{
	ENote::GetInstance().Notify<void>("AutoSetSplitScreen");

	bool isSplit = ENote::GetInstance().Notify<bool>("GetSplitScreen");

	if (isSplit)
	{
		ThisScene->GetCamera()->SetScreenAspect(((float)GameManager::GetInstance().GetScreenWidth() / 2.0f) / (float)GameManager::GetInstance().GetScreenHeight());
		ThisScene->GetCamera()->BuildProjection();
		ThisScene->GetSecCamera()->SetScreenAspect(((float)GameManager::GetInstance().GetScreenWidth() / 2.0f) / (float)GameManager::GetInstance().GetScreenHeight());
		ThisScene->GetSecCamera()->BuildProjection();
	}
	else
	{
		ThisScene->GetCamera()->SetScreenAspect((float)GameManager::GetInstance().GetScreenWidth() / (float)GameManager::GetInstance().GetScreenHeight());
		ThisScene->GetCamera()->BuildProjection();
	}
	

	m_pause = true;
	ThisScene->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/Irradiance/Irradiance.dds"));
	ThisScene->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/earth_moon_skybox.dds"));

	auto dirL = ThisScene->GetLight()->GetModDirectionalLight();

	dirL->LightColor[0] = 1.0f;
	dirL->LightColor[1] = 1.0f;
	dirL->LightColor[2] = 1.0f;

	dirL->LightIntensity[0] = 1.0f;
	dirL->LightIntensity[1] = 0.1f;

	dirL->Direction[0] = 0.1f;
	dirL->Direction[1] = -0.6f;
	dirL->Direction[2] = -0.8f;

	ThisScene->GetCamera()->SetPosition(0.0f, 0.0f, -10.0f);

	m_lineData = LoadLine(Manager->GetFileManager()->LoadFile(CourseDataArray[CourseID::hitorigoto].path));

	auto smoothLine = MathHelper::CatmullromSpline(m_lineData, 20, false);
	auto tangents = MathHelper::CatmullromSpline(m_lineData, 20, true);

	auto obj = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);

	float radius = 1.0f;

	obj->GetRenderer()->Model = Manager->GetModel()->AddTubeFromLineData(smoothLine, tangents, radius, smoothLine);
	obj->GetRenderer()->Material.roughness = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/75.png");
	obj->GetRenderer()->Material.metallic = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/25.png");
	obj->SetName("Tube");
	obj->GetTransform()->TextureScale = Vec3f(40.0f, 3.0f, 1.0f);

	m_playerManager.Release();
	m_playerManager.AddPlayer(Manager, 0, m_lineData, radius, DIFF::HARD);

	if (isSplit)
	{
		m_playerManager.AddPlayer(Manager, 1, m_lineData, radius, DIFF::HARD);
	}
	else
	{
		m_playerManager.AddAI(Manager, 2, m_lineData, radius, DIFF::HARD);
	}
	
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

	if (!m_pause)
	{
		m_playerManager.Update(dt);
	}
}

void PulseGame::Init()
{
	InitTestScene();

	m_result.StartScene("ResultScene");
}

void PulseGame::Update(float dt)
{
	UpdateTestScene(dt);


}
