#include "PulseGame.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/ENote.h"
#include "../External Soruce/cpplinq.hpp"

using namespace cpplinq;

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

	m_stageManager.Initialize(Manager);


	int courseNum = ENote::GetInstance().Notify<int>("GetSelectedSong");
	m_lineData = LoadLine(Manager->GetFileManager()->LoadFile(CourseDataArray[courseNum].path));

	auto smoothLine = MathHelper::CatmullromSpline(m_lineData, 20, false);
	auto tangents = MathHelper::CatmullromSpline(m_lineData, 20, true);

	auto obj = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);

	float radius = 1.3f;

	obj->GetRenderer()->Model = Manager->GetModel()->AddTubeFromLineData(smoothLine, tangents, radius, smoothLine);
	obj->GetRenderer()->Material.roughness = Manager->GetTextureManager()->AddTexture(L"Resource/balls/75.png");
	obj->GetRenderer()->Material.metallic = Manager->GetTextureManager()->AddTexture(L"Resource/balls/25.png");
	obj->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/blue.jpg");
	obj->SetName("Tube");

	obj->GetTransform()->TextureScale = Vec3f(230.0f, 10.0f, 1.0f);

	Manager->GetPackage()->LoadPackage("Packages/ingame/environment.pkg");

	int shipNum;
	std::string note;
	note = "GetPlayer1Ship";
	shipNum = ENote::GetInstance().Notify<int>(note);
	m_playerManager.Release();
	m_playerManager.AddPlayer(Manager, 0, m_lineData, radius, static_cast<DIFF>(MachineDataArray[shipNum].layoutType));
	
	if (isSplit)
	{
		note = "GetPlayer2Ship";
		shipNum = ENote::GetInstance().Notify<int>(note);
		m_playerManager.AddPlayer(Manager, 1, m_lineData, radius, static_cast<DIFF>(MachineDataArray[shipNum].layoutType));
		
	}
	else
	{
		m_playerManager.AddAI(Manager, 2, m_lineData, radius, DIFF::HARD);
	}
	
	//Progress-----------------------
	m_maxProgressValue = (m_lineData.size() - 4) * 0.5f;

	//Load from package
	auto progressPkg = Manager->GetPackage()->LoadPackage("Packages/ingame/progress.pkg");
	m_progressBar = from(progressPkg) >> where([](GameObject* obj) {return obj->GetName() == "progressBar"; }) >> first();
	m_progressPoint1 = from(progressPkg) >> where([](GameObject* obj) {return obj->GetName() == "progressPoint1"; }) >> first();
	m_progressPoint2 = from(progressPkg) >> where([](GameObject* obj) {return obj->GetName() == "progressPoint2"; }) >> first();
	auto endPoint = from(progressPkg) >> where([](GameObject* obj) {return obj->GetName() == "progressEndPoint"; }) >> first();
	m_progressEnd = endPoint->GetTransform()->Position;

	m_progressP1Pos = m_progressPoint1->GetTransform()->Position.y;
	m_progressP2Pos = m_progressPoint2->GetTransform()->Position.y;
	m_progressPosRange = m_progressEnd.y - m_progressP1Pos;
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
		CalculateProgress();
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
	UpdateTestScene(dt);
	m_mainTEween.Update(dt);
}

void PulseGame::ChangeScene()
{
	Manager->ChangeScene("ResultScene");
}

void PulseGame::CalculateProgress()
{
	//Only once per 2 frames
	static bool skip = false;

	skip = !skip;

	//if (skip) return;

	float progressP1 = ENote::GetInstance().Notify<float>("GetPlayerProgress0");
	float progressP2 = ENote::GetInstance().Notify<float>("GetPlayerProgress1");

	ElixirLog("P1: " + std::to_string(progressP1));
	//ElixirLog("P2: " + std::to_string(progressP2));

	float progressDeltaPos1 = (progressP1 * m_progressPosRange) / m_maxProgressValue;
	float progressDeltaPos2 = (progressP2 * m_progressPosRange) / m_maxProgressValue;

	m_progressPoint1->GetTransform()->Position.y = m_progressP1Pos + progressDeltaPos1;
	m_progressPoint2->GetTransform()->Position.y = m_progressP1Pos + progressDeltaPos2;
}
