#include "ModeScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void ModeScene::Init()
{
	SetImage();
	BlackImage();
	StartAnim();

	ModeSelect_Left();
	ModeSelect_Right();
	m_song.StartScene("SongSelect");

	ETween<F32> first_afterTween;
	first_afterTween = first_afterTween.From(&m_modetitle->GetTransform()->Position.y).To(-310.0f).Time(0.7f);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);

}

//Update
void ModeScene::Update(float dt)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		left->Get2DRenderer()->Enabled = true;
		right->Get2DRenderer()->Enabled = false;

	}
	else if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		right->Get2DRenderer()->Enabled = true;
		left->Get2DRenderer()->Enabled = false;

	}

	if (GetAsyncKeyState('X') & 0x8000)
	{
		if (left->Get2DRenderer()->Enabled)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
		}
		else
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
		}

	}

	m_mainTEween.Update(dt);
}

void ModeScene::SetImage()
{
	//画像表示　位置、サイズ
	m_modetitle = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_modetitle->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/mode_title.png");
	m_modetitle->GetTransform()->Position = Vec3f(0, -410, 0);
	m_modetitle->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto single = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	single->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/singleplayer.png");
	single->GetTransform()->Position = Vec3f(-250, -100, 0);
	single->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto multi = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	multi->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/multiplayer.png");
	multi->GetTransform()->Position = Vec3f(250, -100, 0);
	multi->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);
}

void ModeScene::ModeSelect_Left()
{
	left = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	left->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/mode_select.png");
	left->GetTransform()->Position = Vec3f(-250, -100, 0);
	left->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);
	left->Get2DRenderer()->Enabled = false;
}

void ModeScene::ModeSelect_Right()
{
	right = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	right->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/mode_select.png");
	right->GetTransform()->Position = Vec3f(250, -100, 0);
	right->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);
	right->Get2DRenderer()->Enabled = false;
}

void ModeScene::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
}

void ModeScene::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2000.0f).Time(0.7f);
}

void ModeScene::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
}

void ModeScene::ChangeScene()
{
	Manager->ChangeScene("SongSelect");
}
