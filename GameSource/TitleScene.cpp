#include "TitleScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void TitleScene::Init()
{
	SetImage();
	BlackImage();
	StartAnim();

	m_mode.StartScene("ModeScene");

}

//Update
void TitleScene::Update(float dt)
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		BackAnim();
		m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
	}

	m_mainTEween.Update(dt);
}

void TitleScene::SetImage()
{
	//画像表示　位置、サイズ
	auto title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/pulse_title.png");
	title->GetTransform()->Position = Vec3f(0, 150, 0);
	title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto start = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	start->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/start_button.png");
	start->GetTransform()->Position = Vec3f(0, -200, 0);
	start->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);
}

void TitleScene::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
}

void TitleScene::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->Get2DRenderer()->Color.a).To(-1.0f).Time(2.7f);
}

void TitleScene::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_back->Get2DRenderer()->Color.a = 1;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
}


void TitleScene::ChangeScene()
{
	Manager->ChangeScene("ModeScene");
}
