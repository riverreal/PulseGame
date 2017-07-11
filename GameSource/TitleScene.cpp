#include "TitleScene.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../External Soruce/cpplinq.hpp"

using namespace Elixir;
using namespace cpplinq;

//start
void TitleScene::Init()
{
	m_isNextButtonPressed = false;
	m_mainTEween.ReleaseTweens();
	m_crystalTween.ReleaseTweens();
	m_crystalTween2.ReleaseTweens();
	m_inputEnabled = false;

	SetImage();
	BlackImage();
	StartAnim();

	Manager->GetCurrentScene()->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resource/Cubemaps/Irradiance/Irradiance.dds"));
	Manager->GetCurrentScene()->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resource/Cubemaps/earth_moon_skybox.dds"));

	m_mode.StartScene("ModeScene");

	from(Manager->GetCurrentScene()->GetChildren())
		>> where([](GameObject* obj) {return obj->Get2DRenderer() != nullptr; })
		>> for_each([](GameObject* obj) {
		obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale(); 
		obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();
	});
}

//Update
void TitleScene::Update(float dt)
{
	if (m_inputEnabled)
	{
		if (GetAsyncKeyState('Z') & 0x8000 && !m_isNextButtonPressed)
		{
			m_isNextButtonPressed = true;
			BackAnim();
		}
	}
	
	m_mainTEween.Update(dt);
	m_crystalTween.Update(dt);
	m_crystalTween2.Update(dt);
}

void TitleScene::SetImage()
{
	Manager->GetPackage()->LoadPackage("Packages/TitleScene.pkg");

	m_crystal = Manager->GetCurrentScene()->GetObjectByName("Crystal");
	auto whiteBG = Manager->GetCurrentScene()->GetObjectByName("WhiteBG");
	auto pulseTitle = Manager->GetCurrentScene()->GetObjectByName("PulseTitle");

	m_mainTEween = m_mainTEween.From(&m_crystal->Get2DRenderer()->Color.a).To(1.0f).Time(2.0f).OnFinish([this]() { this->CrystalAnimation(true); });
	m_mainTEween = m_mainTEween.From(&pulseTitle->Get2DRenderer()->Color.a).To(1.0f).Time(2.0f);
	m_mainTEween = m_mainTEween.From(&whiteBG->GetTransform()->Scale.y).To(0.0f).Time(1.6f).Easing(EASING_FUNCTION::ET_CUBIC_IN_OUT);
}

void TitleScene::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/balls/0.png");
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

void TitleScene::CrystalAnimation(bool up)
{
	float increment = 20.0f;
	increment *= up ? 1.0f : -1.0f;

	float duration = 1.0f;

	if (up)
	{
		m_crystalTween = m_crystalTween.From(&m_crystal->GetTransform()->Position.y)
			.To(m_crystal->GetTransform()->Position.y + increment).Time(duration).Easing(EASING_FUNCTION::ET_QUINT_IN_OUT)
			.OnFinish([this, up]() {this->CrystalAnimation(!up); });
	}
	else
	{
		m_crystalTween2 = m_crystalTween2.From(&m_crystal->GetTransform()->Position.y)
			.To(m_crystal->GetTransform()->Position.y + increment).Time(duration + 0.2f).Easing(EASING_FUNCTION::ET_QUINT_IN_OUT)
			.OnFinish([this, up]() {this->CrystalAnimation(!up); });
	}
	
}

void TitleScene::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_back->Get2DRenderer()->Color.a = 1;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
	m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
}

void TitleScene::ChangeScene()
{
	Manager->ChangeScene("ModeScene");
}
