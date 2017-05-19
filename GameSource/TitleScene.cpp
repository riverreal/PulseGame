#include "TitleScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void TitleSecne::Init()
{
	SetImage();

	m_mode.StartScene("ModeScene");
	//シーンの移動
	//Manager->ChangeScene("gsgs");
}

//Update
void TitleSecne::Update(float dt)
{
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Manager->ChangeScene("ModeScene");
	}
}

void TitleSecne::SetImage()
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
