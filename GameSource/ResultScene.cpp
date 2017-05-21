#include "ResultScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void ResultScene::Init()
{
	SetImage();
	SetNextButton();

	//シーンの移動
	//Manager->ChangeScene("gsgs");
}

//Update
void ResultScene::Update(float dt)
{
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Manager->ChangeScene("TitleScene");
	}
}


void ResultScene::SetNextButton()
{
	auto next = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	next->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/next_button.png");
	next->GetTransform()->Position = Vec3f(-400, -300, 0);
	next->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);
}

void ResultScene::SetImage()
{
	//画像表示　位置、サイズ

	auto title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/result.title.png");
	title->GetTransform()->Position = Vec3f(0, -380, 0);
	title->GetTransform()->Scale = Vec3f(0.2f, 0.2f, 0);


	auto resultpanel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	resultpanel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/result_panel.png");
	resultpanel->GetTransform()->Position = Vec3f(-300, 100, 0);
	resultpanel->GetTransform()->Scale = Vec3f(0.8f, 0.8f, 0);


	auto player1 = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	player1->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/elem_panel.png");
	player1->GetTransform()->Position = Vec3f(-450, 250, 0);
	player1->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto scorepanel1 = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	scorepanel1->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/elem_panel.png");
	scorepanel1->GetTransform()->Position = Vec3f(-450, 150, 0);
	scorepanel1->GetTransform()->Scale = Vec3f(0.4f, 0.4f, 0);

	auto combopanel1 = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	combopanel1->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/elem_panel.png");
	combopanel1->GetTransform()->Position = Vec3f(-450, 50, 0);
	combopanel1->GetTransform()->Scale = Vec3f(0.4f, 0.4f, 0);

	auto machinepanel1 = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	machinepanel1->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/elem_panel.png");
	machinepanel1->GetTransform()->Position = Vec3f(-450, -50, 0);
	machinepanel1->GetTransform()->Scale = Vec3f(0.4f, 0.4f, 0);


	auto player2 = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	player2->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/elem_panel.png");
	player2->GetTransform()->Position = Vec3f(0, 250, 0);
	player2->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto scorepanel2 = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	scorepanel2->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/elem_panel.png");
	scorepanel2->GetTransform()->Position = Vec3f(0, 150, 0);
	scorepanel2->GetTransform()->Scale = Vec3f(0.4f, 0.4f, 0);

	auto combopanel2 = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	combopanel2->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/elem_panel.png");
	combopanel2->GetTransform()->Position = Vec3f(0, 50, 0);
	combopanel2->GetTransform()->Scale = Vec3f(0.4f, 0.4f, 0);

	auto machinepanel2 = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	machinepanel2->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/elem_panel.png");
	machinepanel2->GetTransform()->Position = Vec3f(0, -50, 0);
	machinepanel2->GetTransform()->Scale = Vec3f(0.4f, 0.4f, 0);


	auto coursepanel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	coursepanel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_name_panel.png");
	coursepanel->GetTransform()->Position = Vec3f(400, -50, 0);
	coursepanel->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto songpanel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	songpanel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_name_panel.png");
	songpanel->GetTransform()->Position = Vec3f(400, -200, 0);
	songpanel->GetTransform()->Scale = Vec3f(0.4f, 0.3f, 0);

	auto lvpanel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	lvpanel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_name_panel.png");
	lvpanel->GetTransform()->Position = Vec3f(400, -300, 0);
	lvpanel->GetTransform()->Scale = Vec3f(0.3f, 0.2f, 0);
}

