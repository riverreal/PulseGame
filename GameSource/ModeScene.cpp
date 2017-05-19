#include "ModeScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void ModeScene::Init()
{
	SetImage();
	//シーンの移動
	//Manager->ChangeScene("gsgs");
}

//Update
void ModeScene::Update(float dt)
{

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

