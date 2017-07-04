#include "TitleScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../External Soruce/cpplinq.hpp"


using namespace Elixir;
using namespace cpplinq;
//using namespace rxcpp;

namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::subjects;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;
}

//start
void TitleScene::Init()
{
	m_isNextButtonPressed = false;
	m_mainTEween.ReleaseTweens();
	m_inputEnabled = false;

	SetImage();
	BlackImage();
	StartAnim();

	m_mode.StartScene("ModeScene");

	from(Manager->GetCurrentScene()->GetChildren())
		>> where([](GameObject* obj) {return obj->Get2DRenderer() != nullptr; })
		>> for_each([](GameObject* obj) {
		obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale(); 
		obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();
	});

	auto nextAsObservable = m_trigger.get_observable();
	auto subscription = m_trigger.get_subscription();
	nextAsObservable.subscribe([this](bool isNext) {
		if (isNext)
		{
			this->BackAnim();
		}
	});
}

//Update
void TitleScene::Update(float dt)
{
	if (m_inputEnabled)
	{
		if (GetAsyncKeyState('Z') & 0x8000)
		{
			m_trigger.get_subscriber().on_next(true);
		}
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
	m_mainTEween = m_mainTEween.From(&m_back->Get2DRenderer()->Color.a).To(-1.0f).Time(1.0f).OnFinish([this]() {this->EnableInput(); });
}

void TitleScene::EnableInput()
{
	m_inputEnabled = true;
}

void TitleScene::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_back->Get2DRenderer()->Color.a = 1;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
	m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
	m_trigger.get_subscription().unsubscribe();
}


void TitleScene::ChangeScene()
{
	Manager->ChangeScene("ModeScene");
}
