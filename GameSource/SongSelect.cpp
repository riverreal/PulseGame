#include "SongSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../External Soruce/cpplinq.hpp"
#include "../Source/Helper/ENote.h"

using namespace Elixir;
using namespace cpplinq;

//start
void SongSelect::Init()
{
	m_inputEnabled = false;
	m_mainTEween.ReleaseTweens();
	auto dirL = Manager->GetCurrentScene()->GetLight()->GetModDirectionalLight();

	dirL->LightColor[0] = 1.0f;
	dirL->LightColor[1] = 1.0f;
	dirL->LightColor[2] = 1.0f;

	dirL->LightIntensity[0] = 2.0f;
	dirL->LightIntensity[1] = 0.003f;
	dirL->Direction[0] = 0.6f;
	dirL->Direction[1] = -0.6f;
	dirL->Direction[2] = 0.8f;

	ENote::GetInstance().Notify<void>("SetNormalScreen");

	SetImage();
	BlackImage();

	StartAnim();

	m_machine.StartScene("MachineSelect");

	Manager->GetCurrentScene()->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/Irradiance/Irradiance.dds"));
	Manager->GetCurrentScene()->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/earth_moon_skybox.dds"));

	from(Manager->GetCurrentScene()->GetChildren())
		>> where([](GameObject* obj) {return obj->Get2DRenderer() != nullptr; })
		>> for_each([](GameObject* obj) {
		obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale();
		obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();
	});
	
	ETween<F32> first_afterTween;
	first_afterTween  = first_afterTween.From(&m_title->GetTransform()->Position.y)
		.To(-310.0f * GameManager::GetInstance().GetDesignScale()).Time(0.3f).Easing(ET_BACK_OUT);
	ETween<F32> afterTween;
	afterTween = afterTween.From(&m_panel->GetTransform()->Position.y)
		.To(285.0f * GameManager::GetInstance().GetDesignScale()).Time(0.5f).Easing(ET_BACK_OUT).OnFinish([this]() {this->EnableInput(); });
	first_afterTween = first_afterTween.OnFinishChain(&afterTween);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);

	auto camera = Manager->GetCurrentScene()->GetCamera();
	camera->SetPosition(XMFLOAT3(20.0f, 1.0f, 1.0f));
	camera->Update();
}

//Update
void SongSelect::Update(float dt)
{
	if (m_inputEnabled)
	{
		if (GetAsyncKeyState('Z') & 0x8000)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
			m_inputEnabled = false;
		}

		if (GetAsyncKeyState('X') & 0x8000)
		{
			
			Log() << Manager->GetCurrentScene()->GetCamera()->GetPosition().x << "\n";
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->BackScene(); });
			m_inputEnabled = false;
		}
	}

	m_mainTEween.Update(dt);
}

void SongSelect::SetImage()
{
	//画像表示　位置、サイズ
	m_title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_sel_title.png");
	m_title->GetTransform()->Position = Vec3f(0, -410, 0);
	m_title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	m_panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_panel.png");
	m_panel->GetTransform()->Position = Vec3f(0, 520, 0);
	m_panel->GetTransform()->Scale = Vec3f(1.2f, 0.5f, 0);

	/*auto circle = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	circle->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/level_circle.png");
	circle->GetTransform()->Position = Vec3f(-300, 50, 0);
	circle->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto elem = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	elem->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/level_elem.png");
	elem->GetTransform()->Position = Vec3f(-300, 50, 0);
	elem->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);*/

	auto back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/back_button.png");
	back->GetTransform()->Position = Vec3f(-500, -300, 0);
	back->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto next = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	next->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/select_button.png");
	next->GetTransform()->Position = Vec3f(500, -300, 0);
	next->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto earth = Manager->GetCurrentScene()->CreateObject(OBJECT_RENDER);
	earth->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);
	earth->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/earth.jpeg");
	earth->GetTransform()->Position = Vec3f(0, 2.5f, 8.0f);
	earth->GetTransform()->Scale = Vec3f(2);
}

void SongSelect::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
	m_back->Get2DRenderer()->Enabled = false;
}

void SongSelect::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2300.0f).Time(0.7f);
}

void SongSelect::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
}

void SongSelect::ChangeScene()
{
	Manager->ChangeScene("MachineSelect");
}

void SongSelect::BackScene()
{
	Manager->ChangeScene("ModeScene");
}

void SongSelect::EnableInput()
{
	m_inputEnabled = true;
}
