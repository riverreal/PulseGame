#include "MachineSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void MachineSelect::Init()
{
	SetImage();
	BlackImage();
	StartAnim();

	m_pulse.StartScene("PulseGame");
	/*
	ETween<F32> first_afterTween;
	first_afterTween = first_afterTween.From(&m_title->GetTransform()->Position.y).To(-310.0f).Time(0.7f);
	ETween<F32> afterTween;
	afterTween = afterTween.From(&m_panel->GetTransform()->Position.x).To(600.0f).Time(0.7f).Easing(ET_BACK_IN_OUT);
	first_afterTween = first_afterTween.OnFinishChain(&afterTween);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);
	*/
}

//Update
void MachineSelect::Update(float dt)
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		BackAnim();
		m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
	}

	if (GetAsyncKeyState('X') & 0x8000)
	{
		BackAnim();
		m_mainTEween = m_mainTEween.OnFinish([this]() {this->BackScene(); });
	}

	m_mainTEween.Update(dt);
}

void MachineSelect::SetImage()
{
	//画像表示　位置、サイズ
	m_title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_sel_title.png");
	m_title->GetTransform()->Position = Vec3f(0, -380, 0);
	m_title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	m_panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_panel.png");
	m_panel->GetTransform()->Position = Vec3f(800, 0, 0);
	m_panel->GetTransform()->Scale = Vec3f(0.5f, 0.7f, 0);


	auto back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/back_button.png");
	back->GetTransform()->Position = Vec3f(-400, -250, 0);
	back->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto next = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	next->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/next_button.png");
	next->GetTransform()->Position = Vec3f(400, -250, 0);
	next->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	/*auto bar = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	bar->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_status_bar.png");
	bar->GetTransform()->Position = Vec3f(500, -200, 0);
	bar->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);*/
}

void MachineSelect::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
}

void MachineSelect::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2000.0f).Time(0.7f);
}

void MachineSelect::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
}

void MachineSelect::ChangeScene()
{
	Manager->ChangeScene("PulseGame");
}

void MachineSelect::BackScene()
{
	Manager->ChangeScene("SongSelect");
}

