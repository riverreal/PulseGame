#include "ModeScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/ENote.h"
#include "../External Soruce/cpplinq.hpp"

using namespace Elixir;
using namespace cpplinq;

//start
void ModeScene::Init()
{
	auto camera = Manager->GetCurrentScene()->GetCamera();
	auto camPos = camera->GetPosition();
	camPos.z -= 5.0f;
	camera->SetPosition(camPos);

	m_mainTEween.ReleaseTweens();
	m_targetTween.ReleaseTweens();
	m_inputEnabled = false;
	m_splitScreen = false;
	m_singlePlayerMode = true;
	m_waitFrame = 0;

	auto dirL = Manager->GetCurrentScene()->GetLight()->GetModDirectionalLight();

	dirL->LightColor[0] = 1.0f;
	dirL->LightColor[1] = 1.0f;
	dirL->LightColor[2] = 1.0f;

	dirL->LightIntensity[0] = 2.0f;
	dirL->LightIntensity[1] = 0.5f;
	dirL->Direction[0] = -0.6f;
	dirL->Direction[1] = -0.6f;
	dirL->Direction[2] = -0.8f;

	Manager->GetCurrentScene()->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resource/Cubemaps/Irradiance/Irradiance.dds"));
	Manager->GetCurrentScene()->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resource/Cubemaps/earth_moon_skybox.dds"));

	//Set UI-------------------------------
	SetImage();

	BlackImage();
	StartAnim();

	from(Manager->GetCurrentScene()->GetChildren())
		>> where([](GameObject* obj) {return obj->Get2DRenderer() != nullptr; })
		>> for_each([](GameObject* obj) {
		obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale();
		obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();
	});

	m_song.StartScene("SongSelect");

	ENote::GetInstance().AddNote<bool>("GetSplitScreen", [this]() {return this->GetSplitScreen(); });
	
	ETween<F32> first_afterTween;
	first_afterTween = first_afterTween.From(&m_modetitle->GetTransform()->Position.y).To(-310.0f * GameManager::GetInstance().GetDesignScale())
		.Time(0.4f).OnFinish([this]() {this->EnableInput(); }).Easing(ET_BACK_OUT);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);

	m_camPos.x = camPos.x;
	m_camPos.y = camPos.y;
	m_camPos.z = camPos.z;
	m_mainTEween = m_mainTEween.From(&m_camPos.z).To(m_camPos.z + 5.0f).Time(3.0f).Easing(ET_BACK_OUT);
	m_camLookAt = Vec3f(0.0f, 4.0f, 10.0f);

	DirectX::XMFLOAT3 target = XMFLOAT3(m_camLookAt.x, m_camLookAt.y, m_camLookAt.z);
	camera->SetLookAt(camera->GetPosition(), target, XMFLOAT3(0.0f, 1.0f, 0.0f));

	auto singleTargetObj = Manager->GetCurrentScene()->GetObjectByName("SingleTarget");
	m_singleTarget = singleTargetObj->GetTransform()->Position;
	auto multiTargetObj = Manager->GetCurrentScene()->GetObjectByName("MultiTarget");
	m_multiTarget = multiTargetObj->GetTransform()->Position;

	m_targetTween = m_targetTween.From(&m_camLookAt).To(m_singleTarget).Time(3.0f).Easing(ET_BACK_OUT);

	camera->Update();
}

//Update
void ModeScene::Update(float dt)
{
	if (m_inputEnabled)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !m_singlePlayerMode)
		{
			m_targetTween = m_targetTween.From(&m_camLookAt).To(m_singleTarget).Time(0.5f).Easing(ET_BACK_OUT);
			m_singlePlayerMode = true;
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && m_singlePlayerMode)
		{
			m_targetTween = m_targetTween.From(&m_camLookAt).To(m_multiTarget).Time(0.5f).Easing(ET_BACK_OUT);
			m_singlePlayerMode = false;
		}

		if (GetAsyncKeyState('Z') & 0x8000)
		{
			
			if (m_singlePlayerMode)
			{
				m_splitScreen = false;
				BackAnim();
				m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
				m_inputEnabled = false;
			}
			else if (m_singlePlayerMode)
			{
				m_splitScreen = true;
				BackAnim();
				m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
				m_inputEnabled = false;
			}
			
		}
	}

	auto cam = Manager->GetCurrentScene()->GetCamera();
	cam->SetPosition(m_camPos.x, m_camPos.y, m_camPos.z);
	DirectX::XMFLOAT3 target = XMFLOAT3(m_camLookAt.x, m_camLookAt.y, m_camLookAt.z);
	cam->SetLookAt(cam->GetPosition(), target, XMFLOAT3(0.0f, 1.0f, 0.0f));

	if (m_waitFrame < 10)
	{
		m_waitFrame++;
	}
	else
	{
		m_targetTween.Update(dt);
		m_mainTEween.Update(dt);
	}
	
}

void ModeScene::SetImage()
{
	Manager->GetPackage()->LoadPackage("Packages/ModeScene.pkg");
	
	m_modetitle = Manager->GetCurrentScene()->GetObjectByName("ModeTitle");
}

bool ModeScene::GetSplitScreen()
{
	return m_splitScreen;
}

void ModeScene::EnableInput()
{
	m_inputEnabled = true;
}

void ModeScene::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
}

void ModeScene::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-1500.0f).Time(0.4f);
}

void ModeScene::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.4f);
}

void ModeScene::ChangeScene()
{
	Manager->ChangeScene("SongSelect");
}