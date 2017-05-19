#include "SongSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void SongSelect::Init()
{
	SetImage();
	//�V�[���̈ړ�
	//Manager->ChangeScene("gsgs");
}

//Update
void SongSelect::Update(float dt)
{

}

void SongSelect::SetImage()
{
	//�摜�\���@�ʒu�A�T�C�Y
	auto title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_sel_title.png");
	title->GetTransform()->Position = Vec3f(0, -400, 0);
	title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_panel.png");
	panel->GetTransform()->Position = Vec3f(450, 0, 0);
	panel->GetTransform()->Scale = Vec3f(0.5f, 0.7f, 0);

	auto bar = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	bar->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_status_bar.png");
	bar->GetTransform()->Position = Vec3f(500, -200, 0);
	bar->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);
}

