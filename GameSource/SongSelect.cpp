#include "SongSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../External Soruce/cpplinq.hpp"
#include "../Source/Helper/ENote.h"
#include "CourseSelect.h"

using namespace Elixir;
using namespace cpplinq;

//start
void SongSelect::Init()
{
	m_inputEnabled = false;
	m_mainTEween.ReleaseTweens();
	m_selectedSong = 0;
	auto dirL = Manager->GetCurrentScene()->GetLight()->GetModDirectionalLight();

	dirL->LightColor[0] = 1.0f;
	dirL->LightColor[1] = 1.0f;
	dirL->LightColor[2] = 1.0f;

	dirL->LightIntensity[0] = 2.0f;
	dirL->LightIntensity[1] = 0.003f;
	dirL->Direction[0] = 0.6f;
	dirL->Direction[1] = -0.6f;
	dirL->Direction[2] = 0.8f;

	ENote::GetInstance().Notify<void>("SetNormalScreen");
	ENote::GetInstance().AddNote<int>("GetSelectedSong", [this]() {return this->GetSelectedSong(); });

	SetImage();
	BlackImage();

	StartAnim();

	SetPlanets();

	m_machine.StartScene("MachineSelect");

	Manager->GetCurrentScene()->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/Irradiance/Irradiance.dds"));
	Manager->GetCurrentScene()->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/earth_moon_skybox.dds"));

	from(Manager->GetCurrentScene()->GetChildren())
		>> where([](GameObject* obj) {return obj->Get2DRenderer() != nullptr; })
		>> for_each([](GameObject* obj) {
		obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale();
		obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();
	});
	
	ETween<F32> first_afterTween;
	first_afterTween  = first_afterTween.From(&m_title->GetTransform()->Position.y)
		.To(-310.0f * GameManager::GetInstance().GetDesignScale()).Time(0.3f).Easing(ET_BACK_OUT);
	ETween<F32> afterTween;
	afterTween = afterTween.From(&m_panel->GetTransform()->Position.y)
		.To(285.0f * GameManager::GetInstance().GetDesignScale()).Time(0.5f).Easing(ET_BACK_OUT).OnFinish([this]() {this->EnableInput(); });
	first_afterTween = first_afterTween.OnFinishChain(&afterTween);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);

	auto camera = Manager->GetCurrentScene()->GetCamera();
	camera->SetPosition(XMFLOAT3(20.0f, 0.0f, 1.0f));
	camera->Update();

	m_camPos = Vec3f(0, camera->GetPosition().y, camera->GetPosition().z);

	AudioManager::GetInstance().PlayMusic("Resource/rhythmFolder/" + CourseDataArray[0].music + ".mp3");
	m_maxSelSongs = sizeof(CourseDataArray) / sizeof(CourseDataArray[0]);
}

//Update
void SongSelect::Update(float dt)
{
	if (m_inputEnabled)
	{
		if (GetAsyncKeyState('Z') & 0x8000)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
			m_inputEnabled = false;
		}

		if (GetAsyncKeyState('X') & 0x8000)
		{	
			Log() << Manager->GetCurrentScene()->GetCamera()->GetPosition().x << "\n";
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->BackScene(); });
			m_inputEnabled = false;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && m_inputEnabled)
		{
			if (m_selectedSong < m_maxSelSongs - 1)
			{
				m_inputEnabled = false;
				m_selectedSong++;
				m_mainTEween = m_mainTEween.From(&m_camPos.x).To(m_selectedSong*6.0f).Time(0.4f).Easing(ET_CUBIC_IN_OUT)
					.OnFinish([this]() {this->EnableInput(); });
				m_panel->GetTransform()->Position.y = 520;
				m_mainTEween = m_mainTEween.From(&m_panel->GetTransform()->Position.y)
					.To(285.0f * GameManager::GetInstance().GetDesignScale()).Time(0.5f).Easing(ET_BACK_OUT);
				AudioManager::GetInstance().PlayMusic("Resource/rhythmFolder/" + CourseDataArray[m_selectedSong].music + ".mp3");
				AudioManager::GetInstance().PlaySoundEffect("Resource/SoundEffect/SelectScene/select09.mp3");
			}
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && m_inputEnabled)
		{
			if (m_selectedSong > 0)
			{
				m_inputEnabled = false;
				m_selectedSong--;
				m_mainTEween = m_mainTEween.From(&m_camPos.x).To(m_selectedSong*6.0f).Time(0.4f).Easing(ET_CUBIC_IN_OUT)
					.OnFinish([this]() {this->EnableInput(); });
				m_panel->GetTransform()->Position.y = 520;
				m_mainTEween = m_mainTEween.From(&m_panel->GetTransform()->Position.y)
					.To(285.0f * GameManager::GetInstance().GetDesignScale()).Time(0.5f).Easing(ET_BACK_OUT);
				
				AudioManager::GetInstance().PlayMusic("Resource/rhythmFolder/" + CourseDataArray[m_selectedSong].music + ".mp3");
				AudioManager::GetInstance().PlaySoundEffect("Resource/SoundEffect/select09.mp3");

			}

		}
	}

	m_planets->GetChildren()[m_selectedSong]->GetTransform()->Rotation.y += 50.0f * dt;

	auto cam = Manager->GetCurrentScene()->GetCamera();
	cam->SetPosition(m_camPos.x, m_camPos.y, m_camPos.z);

	m_mainTEween.Update(dt);
}

void SongSelect::SetImage()
{
	//画像表示　位置、サイズ
	m_title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/course_sel_title.png");
	m_title->GetTransform()->Position = Vec3f(0, -410, 0);
	m_title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	m_panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/CourseScene/ship_panel2.png");
	m_panel->GetTransform()->Position = Vec3f(0, 520, 0);
	m_panel->GetTransform()->Scale = Vec3f(0.9f, 0.5f, 0);

	/*auto circle = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	circle->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/level_circle.png");
	circle->GetTransform()->Position = Vec3f(-300, 50, 0);
	circle->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	auto elem = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	elem->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/level_elem.png");
	elem->GetTransform()->Position = Vec3f(-300, 50, 0);
	elem->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);*/

	auto back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/back_button.png");
	back->GetTransform()->Position = Vec3f(-500, -300, 0);
	back->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto next = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	next->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/select_button.png");
	next->GetTransform()->Position = Vec3f(500, -300, 0);
	next->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);
}



void SongSelect::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
	m_back->Get2DRenderer()->Enabled = false;
}

void SongSelect::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2300.0f).Time(0.7f);
}

void SongSelect::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
}

void SongSelect::ChangeScene()
{
	Manager->ChangeScene("MachineSelect");
}

void SongSelect::BackScene()
{
	Manager->ChangeScene("ModeScene");
}

void SongSelect::EnableInput()
{
	m_inputEnabled = true;
}

void SongSelect::SetPlanets()
{
	m_planets = Manager->GetCurrentScene()->CreateObject(OBJECT_TRANSFORM);
	m_planets->SetName("Planets");
	m_planets->GetTransform()->Position.y = -0.160;

	int planetCount = sizeof(CourseDataArray) / sizeof(CourseDataArray[0]);

	for (int i = 0; i < planetCount; ++i)
	{
		auto planet = Manager->GetCurrentScene()->CreateObject(OBJECT_RENDER);
		planet->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);
		planet->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(s2ws(CourseDataArray[i].planetSkinPath));
		planet->GetRenderer()->Material.roughness = Manager->GetTextureManager()->AddTexture(L"Resource/balls/200.png");
		planet->GetTransform()->Position.z += 3.5f;
		planet->GetTransform()->Position.x = i * 6.0f;
		planet->SetName("Planet" + std::to_string(i));
		m_planets->AddChild(planet);
	}
}

int SongSelect::GetSelectedSong()
{
	return m_selectedSong;
}
