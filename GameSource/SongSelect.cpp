#include "SongSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

//start
void SongSelect::Init()
{
	SetImage();
	m_machine.StartScene("MachineSelect");

	//シーンの移動
	//Manager->ChangeScene("gsgs");
}

//Update
void SongSelect::Update(float dt)
{
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Manager->ChangeScene("MachineSelect");
	}
}

void SongSelect::SetImage()
{
	//画像表示　位置、サイズ
	auto  title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_sel_title.png");
	title->GetTransform()->Position = Vec3f(150, -400, 0);
	title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_panel.png");
	panel->GetTransform()->Position = Vec3f(0, 50, 0);
	panel->GetTransform()->Scale = Vec3f(1.5f, 0.5f, 0);

	auto circle = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	circle->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/level_circle.png");
	circle->GetTransform()->Position = Vec3f(-300, 50, 0);
	circle->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto elem = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	elem->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/level_elem.png");
	elem->GetTransform()->Position = Vec3f(-300, 50, 0);
	elem->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);
}

