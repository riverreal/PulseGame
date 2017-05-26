#include "SongSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void SongSelect::Init()
{

	SetImage();
	BlackImage();

	StartAnim();

	m_machine.StartScene("MachineSelect");

	ThisScene->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/Irradiance/Irradiance.dds"));
	ThisScene->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/earth_moon_skybox.dds"));

	ETween<F32> first_afterTween;
	first_afterTween  = first_afterTween.From(&m_title->GetTransform()->Position.y).To(-550.0f).Time(0.7f);
	ETween<F32> afterTween;
	afterTween = afterTween.From(&m_panel->GetTransform()->Position.y).To(50.0f).Time(0.7f).Easing(ET_BACK_IN_OUT);
	first_afterTween = first_afterTween.OnFinishChain(&afterTween);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);

	//シーンの移動
	//Manager->ChangeScene("gsgs");
}

//Update
void SongSelect::Update(float dt)
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		BackAnim();
		m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		Manager->ChangeScene("ModeScene");
	}

	m_mainTEween.Update(dt);
}

void SongSelect::SetImage()
{
	//画像表示　位置、サイズ
	m_title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_sel_title.png");
	m_title->GetTransform()->Position = Vec3f(0, -600, 0);
	m_title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	m_panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_panel.png");
	m_panel->GetTransform()->Position = Vec3f(0, 200, 0);
	m_panel->GetTransform()->Scale = Vec3f(1.2f, 0.5f, 0);

	auto circle = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	circle->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/level_circle.png");
	circle->GetTransform()->Position = Vec3f(-300, 50, 0);
	circle->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto elem = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	elem->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/level_elem.png");
	elem->GetTransform()->Position = Vec3f(-300, 50, 0);
	elem->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/back_button.png");
	back->GetTransform()->Position = Vec3f(-400,-200 , 0);
	back->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto earth = Manager->GetCurrentScene()->CreateObject(OBJECT_RENDER);
	earth->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);
	earth->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/earth.jpeg");
	earth->GetTransform()->Position = Vec3f(1.4, 0, 5);
}

void SongSelect::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
}

void SongSelect::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2000.0f).Time(0.7f);
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