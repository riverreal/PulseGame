#include "MachineSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void MachineSelect::Init()
{
	SetImage();

	m_pulse.StartScene("PulseGame");
	//シーンの移動
	//Manager->ChangeScene("gsgs");

	m_mainTEween = m_mainTEween.From(&m_title->GetTransform()->Position.y).To(-550.0f).Time(0.7f);
	ETween<float> afterTween;
	afterTween = afterTween.From(&m_panel->GetTransform()->Position.x).To(450.0f).Time(0.7f);
	m_mainTEween = m_mainTEween.OnFinishChain(&afterTween);
}

//Update
void MachineSelect::Update(float dt)
{
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Manager->ChangeScene("PulseGame");
	}

	m_mainTEween.Update(dt);
}

void MachineSelect::SetImage()
{
	//画像表示　位置、サイズ
	m_title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_sel_title.png");
	m_title->GetTransform()->Position = Vec3f(0, -600, 0);
	m_title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	m_panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_panel.png");
	m_panel->GetTransform()->Position = Vec3f(650, 0, 0);
	m_panel->GetTransform()->Scale = Vec3f(0.5f, 0.7f, 0);

	auto bar = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	bar->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_status_bar.png");
	bar->GetTransform()->Position = Vec3f(500, -200, 0);
	bar->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);
}

