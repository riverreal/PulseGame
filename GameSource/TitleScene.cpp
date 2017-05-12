#include "TitleScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void title::Init()
{
	SetImage();
	//シーンの移動
	//Manager->ChangeScene("gsgs");
}

//Update
void title::Update(float dt)
{

}

void title::SetImage()
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

