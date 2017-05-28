#include "ModeScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/ENote.h"

using namespace Elixir;

//start
void ModeScene::Init()
{
	m_splitScreen = false;
	SetImage();
	//シーンの移動
	//Manager->ChangeScene("gsgs");
	ModeSelect_Left();
	ModeSelect_Right();
	m_song.StartScene("SongSelect");

	ENote::GetInstance().AddNote<bool>("GetSplitScreen", [this]() {return this->GetSplitScreen(); });
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

	if (GetAsyncKeyState('A') & 0x8000)
	{
		if (left->Get2DRenderer()->Enabled)
		{
			m_splitScreen = false;
			Manager->ChangeScene("SongSelect");
		}
	}
	else if(GetAsyncKeyState('D') & 0x8000)
	{
		if (right->Get2DRenderer()->Enabled)
		{
			m_splitScreen = true;
			Manager->ChangeScene("SongSelect");
		}
	}
}

void ModeScene::SetImage()
{
	//画像表示　位置、サイズ
	auto modetitle = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	modetitle->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/mode_title.png");
	modetitle->GetTransform()->Position = Vec3f(150, -400, 0);
	modetitle->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto single = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	single->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/singleplayer.png");
	single->GetTransform()->Position = Vec3f(-250, -170, 0);
	single->GetTransform()->Scale = Vec3f(0.3f, 0.3f, 0);

	auto multi = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	multi->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/multiplayer.png");
	multi->GetTransform()->Position = Vec3f(250, -170, 0);
	multi->GetTransform()->Scale = Vec3f(0.3f, 0.3f, 0);
}

void ModeScene::ModeSelect_Left()
{
	left = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	left->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/mode_select.png");
	left->GetTransform()->Position = Vec3f(-250, -170, 0);
	left->GetTransform()->Scale = Vec3f(0.3f, 0.3f, 0);
	left->Get2DRenderer()->Enabled = false;
}

void ModeScene::ModeSelect_Right()
{
	right = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	right->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/mode_select.png");
	right->GetTransform()->Position = Vec3f(250, -170, 0);
	right->GetTransform()->Scale = Vec3f(0.3f, 0.3f, 0);
	right->Get2DRenderer()->Enabled = false;
}

bool ModeScene::GetSplitScreen()
{
	return m_splitScreen;
}

