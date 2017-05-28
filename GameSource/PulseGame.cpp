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
		Manager->GetCurrentScene()->GetCamera()->SetScreenAspect(((float)GameManager::GetInstance().GetScreenWidth() / 2.0f) / (float)GameManager::GetInstance().GetScreenHeight());
		Manager->GetCurrentScene()->GetCamera()->BuildProjection();
		Manager->GetCurrentScene()->GetSecCamera()->SetScreenAspect(((float)GameManager::GetInstance().GetScreenWidth() / 2.0f) / (float)GameManager::GetInstance().GetScreenHeight());
		Manager->GetCurrentScene()->GetSecCamera()->BuildProjection();
	}
	else
	{
		Manager->GetCurrentScene()->GetCamera()->SetScreenAspect((float)GameManager::GetInstance().GetScreenWidth() / (float)GameManager::GetInstance().GetScreenHeight());
		Manager->GetCurrentScene()->GetCamera()->BuildProjection();
	}
	

	m_pause = true;
	Manager->GetCurrentScene()->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/Irradiance/Irradiance.dds"));
	Manager->GetCurrentScene()->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/earth_moon_skybox.dds"));

	auto dirL = Manager->GetCurrentScene()->GetLight()->GetModDirectionalLight();

	dirL->LightColor[0] = 1.0f;
	dirL->LightColor[1] = 1.0f;
	dirL->LightColor[2] = 1.0f;

	dirL->LightIntensity[0] = 1.0f;
	dirL->LightIntensity[1] = 0.1f;

	dirL->Direction[0] = 0.1f;
	dirL->Direction[1] = -0.6f;
	dirL->Direction[2] = -0.8f;

	Manager->GetCurrentScene()->GetCamera()->SetPosition(0.0f, 0.0f, -10.0f);

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

void PulseGame::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
}

void PulseGame::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2000.0f).Time(0.7f);
}

void PulseGame::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
}

void PulseGame::Init()
{
	InitTestScene();

	BlackImage();
	StartAnim();

	m_result.StartScene("ResultScene");


}

void PulseGame::Update(float dt)
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		BackAnim();
		m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
	}

	UpdateTestScene(dt);
	m_mainTEween.Update(dt);
}

void PulseGame::ChangeScene()
{
	Manager->ChangeScene("ResultScene");
}
