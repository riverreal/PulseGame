#include "ResultScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/ENote.h"

using namespace Elixir;

//start
void ResultScene::Init()
{
	ENote::GetInstance().Notify<void>("SetNormalScreen");

	/*
	SetImage();
	BlackImage();
	StartAnim();

	ETween<F32> first_afterTween;
	first_afterTween = first_afterTween.From(&m_title->GetTransform()->Position.y).To(-310.0f).Time(0.7f);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);

	//SetImage();
	//SetNextButton();
	*/

	TextImage();
	AudioManager::GetInstance().GetControlledMusic()->setIsPaused(true);
}

//Update
void ResultScene::Update(float dt)
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		//BackAnim();
		//m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
		ChangeScene();
	}

	m_mainTEween.Update(dt);
	m_MachineAnim.Update(dt);
	m_textAnim.Update(dt);
	m_textAnim2.Update(dt);
}

void ResultScene::TextImage()
{
	m_text = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_text->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/winner.png");
	m_text->GetTransform()->Position = Vec3f(0, -200, 0);
	//text->GetTransform()->Scale = Vec3f(0.2f, 0.2f, 0);


	m_Machine = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);
	m_Machine->GetRenderer()->Model = Manager->GetModel()->AddModelFromFile("Resource/ships/shipImp.obj");
	m_Machine->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/ships/shipAlbedoEm.png");
	m_Machine->GetRenderer()->Material.metallic = Manager->GetTextureManager()->AddTexture(L"Resource/ships/shipMetallic.png");
	m_Machine->GetRenderer()->Material.normal = Manager->GetTextureManager()->AddTexture(L"Resource/ships/shipNormal.png");
	m_Machine->GetRenderer()->Material.emissive = Manager->GetTextureManager()->AddTexture(L"Resource/ships/shipEmissive.png");
	m_Machine->GetRenderer()->Material.roughness = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/75.png");
	m_Machine->GetTransform()->Scale = Vec3f(0.3f);
	m_Machine->GetTransform()->Position = Vec3f(0, 0, 5);
	m_Machine->GetTransform()->Rotation = Vec3f(0, 30, 0);

	DirectX::XMFLOAT3 playerPos, upVec;
	playerPos.x = m_Machine->GetTransform()->Position.x;
	playerPos.y = m_Machine->GetTransform()->Position.y; 
	playerPos.z = m_Machine->GetTransform()->Position.z;
	upVec.x = 0; upVec.y = 1; upVec.z = 0;
	Manager->GetCurrentScene()->GetCamera()->SetPosition(0, 3, 0);
	Manager->GetCurrentScene()->GetCamera()->SetLookAt(Manager->GetCurrentScene()->GetCamera()->GetPosition(), playerPos, upVec);
	m_Machine->GetTransform()->Position = Vec3f(60, 0, 60);
	m_MachineAnim = m_MachineAnim.From(&m_Machine->GetTransform()->Position).To(Vec3f(0, 0, 5)).Time(4.0f).Easing(ET_QUINT_OUT);
	TextAnimLoop(true);
}

void ResultScene::TextAnimLoop(bool flag)
{
	if (flag)
	{
		m_textAnim = m_textAnim.From(&m_text->GetTransform()->Scale).To(Vec3f(0.9f, 0.9f, 1.0f)).Time(1.0f)
			.OnFinish([this]() {this->TextAnimLoop(false); });
	}
	else
	{
		m_textAnim2 = m_textAnim2.From(&m_text->GetTransform()->Scale).To(Vec3f(1.1f, 1.1f, 1.0f)).Time(1.0f)
			.OnFinish([this]() {this->TextAnimLoop(true); });
	}
}


void ResultScene::SetImage()
{
	//画像表示　位置、サイズ

	m_title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/result.title.png");
	m_title->GetTransform()->Position = Vec3f(0, -410, 0);
	m_title->GetTransform()->Scale = Vec3f(0.2f, 0.2f, 0);


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

	auto next = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	next->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/next_button.png");
	next->GetTransform()->Position = Vec3f(400, -250, 0);
	next->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);
}

void ResultScene::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
}

void ResultScene::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2000.0f).Time(0.7f);
}

void ResultScene::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
}

void ResultScene::ChangeScene()
{
	Manager->ChangeScene("TitleScene");
}

