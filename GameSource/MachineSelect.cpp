#include "MachineSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../External Soruce/cpplinq.hpp"

using namespace Elixir;
using namespace cpplinq;
//start
void MachineSelect::Init()
{
	m_mainTEween.ReleaseTweens();
	m_inputEnabled = false;
	SetImage();
	BlackImage();
	StartAnim();

	m_pulse.StartScene("PulseGame");
	
	from(Manager->GetCurrentScene()->GetChildren())
		>> where([](GameObject* obj) {return obj->Get2DRenderer() != nullptr; })
		>> for_each([](GameObject* obj) {
		obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale();
		obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();
	});

	ETween<F32> first_afterTween;
	first_afterTween = first_afterTween.From(&m_title->GetTransform()->Position.y)
		.To(-310.0f * GameManager::GetInstance().GetDesignScale()).Time(0.3f).Easing(ET_BACK_OUT);
	ETween<F32> afterTween;
	afterTween = afterTween.From(&m_panel->GetTransform()->Position.x)
		.To(600.0f * GameManager::GetInstance().GetDesignScale()).Time(0.5f).Easing(ET_BACK_OUT).OnFinish([this]() {this->EnableInput(); });
	first_afterTween = first_afterTween.OnFinishChain(&afterTween);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);
}

//Update
void MachineSelect::Update(float dt)
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
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->BackScene(); });
			m_inputEnabled = false;
		}
	}

	m_mainTEween.Update(dt);
}

void MachineSelect::SetImage()
{
	//画像表示　位置、サイズ
	m_title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_sel_title.png");
	m_title->GetTransform()->Position = Vec3f(0, -410, 0);
	m_title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	m_panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_panel.png");
	m_panel->GetTransform()->Position = Vec3f(850, 0, 0);
	m_panel->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/back_button.png");
	back->GetTransform()->Position = Vec3f(-500, -300, 0);
	back->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto next = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	next->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/next_button.png");
	next->GetTransform()->Position = Vec3f(500, -300, 0);
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
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2300.0f).Time(0.7f);
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

void MachineSelect::EnableInput()
{
	m_inputEnabled = true;
}

