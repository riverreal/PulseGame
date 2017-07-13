#include "MachineSelect.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../External Soruce/cpplinq.hpp"
#include "../Source/Helper/ENote.h"
#include "CourseSelect.h"

using namespace Elixir;
using namespace cpplinq;

//start
void MachineSelect::Init()
{
	m_mainTEween.ReleaseTweens();

	m_maxShips = sizeof(MachineDataArray) / sizeof(MachineDataArray[0]);

	m_selectedShip = 0;

	m_inputEnabled = false;
	SetImage();
	BlackImage();
	StartAnim();

	auto dirL = Manager->GetCurrentScene()->GetLight()->GetModDirectionalLight();

	dirL->LightColor[0] = 1.0f;
	dirL->LightColor[1] = 1.0f;
	dirL->LightColor[2] = 1.0f;

	dirL->LightIntensity[0] = 2.0f;
	dirL->LightIntensity[1] = 0.3f;
	dirL->Direction[0] = 0.6f;
	dirL->Direction[1] = -0.6f;
	dirL->Direction[2] = 0.8f;

	Manager->GetCurrentScene()->SetIrradiance(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/Irradiance/Irradiance.dds"));
	Manager->GetCurrentScene()->SetEnvMap(Manager->GetTextureManager()->AddTexture(L"Resources/Textures/Cubemaps/earth_moon_skybox.dds"));

	Manager->GetCurrentScene()->GetCamera()->SetPosition(0, 0, 0);

	m_pulse.StartScene("PulseGame");

	m_twoPlayerMode = ENote::GetInstance().Notify<bool>("GetSplitScreen");

	ENote::GetInstance().AddNote<int>("GetPlayer1Ship", [this]() { return this->GetPlayer1Ship(); });
	ENote::GetInstance().AddNote<int>("GetPlayer2Ship", [this]() { return this->GetPlayer2Ship(); });
	
	from(Manager->GetCurrentScene()->GetChildren())
		>> where([](GameObject* obj) {return obj->Get2DRenderer() != nullptr; })
		>> for_each([](GameObject* obj) {
		obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale();
		obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();
	});

	ETween<F32> first_afterTween;
	first_afterTween = first_afterTween.From(&m_title->GetTransform()->Position.y)
		.To(-310.0f * GameManager::GetInstance().GetDesignScale()).Time(0.3f).Easing(ET_BACK_OUT);
	ETween<F32> afterTween;
	afterTween = afterTween.From(&m_panel->GetTransform()->Position.x)
		.To(600.0f * GameManager::GetInstance().GetDesignScale()).Time(0.5f).Easing(ET_BACK_OUT).OnFinish([this]() {this->EnableInput(); });
	first_afterTween = first_afterTween.OnFinishChain(&afterTween);
	m_mainTEween = m_mainTEween.OnFinishChain(&first_afterTween);
}

//Update
void MachineSelect::Update(float dt)
{
	if (m_inputEnabled)
	{
#if ELIXIR_EDITOR
		if (GetAsyncKeyState('Z') & 0x8000)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
			m_inputEnabled = false;
		}

		if (GetAsyncKeyState('X') & 0x8000)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->BackScene(); });
			m_inputEnabled = false;
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && m_selectedShip > 0)
		{
			m_inputEnabled = false;
			m_selectedShip--;
			m_mainTEween = m_mainTEween.From(&m_ships->GetTransform()->Position.x).To(-m_selectedShip * 6.0f).Time(0.4f)
				.OnFinish([this]() {this->EnableInput(); });

			m_mainTEween = m_mainTEween.From(&m_panel->GetTransform()->Position.x)
				.To(600.0f * GameManager::GetInstance().GetDesignScale()).Time(0.4f).Easing(ET_BACK_OUT);
			
			ElixirLog("SelectedShip: " + std::to_string(m_selectedShip));
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && m_selectedShip < m_maxShips - 1)
		{
			m_inputEnabled = false;
			m_selectedShip++;

			m_mainTEween = m_mainTEween.From(&m_ships->GetTransform()->Position.x).To(-m_selectedShip * 6.0f).Time(0.4f)
				.OnFinish([this]() {this->EnableInput(); });

			m_mainTEween = m_mainTEween.From(&m_panel->GetTransform()->Position.x)
				.To(600.0f * GameManager::GetInstance().GetDesignScale()).Time(0.4f).Easing(ET_BACK_OUT);
			
			
			ElixirLog("SelectedShip: " + std::to_string(m_selectedShip));
		}

#else
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && m_selectionNumber == 0)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
			m_inputEnabled = false;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && m_selectionNumber == 0)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->BackScene(); });
			m_inputEnabled = false;
		}

		if (GetAsyncKeyState('N') & 0x8000 && m_selectionNumber == 1)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->ChangeScene(); });
			m_inputEnabled = false;

		}

		if (GetAsyncKeyState('M') & 0x8000 && m_selectionNumber == 1)
		{
			BackAnim();
			m_mainTEween = m_mainTEween.OnFinish([this]() {this->BackScene(); });
			m_inputEnabled = false;
		}

		if (GetAsyncKeyState('Q') & 0x8000 && m_selectedShip > 0 && m_selectionNumber == 0)
		{
			m_inputEnabled = false;
			m_selectedShip--;
			m_mainTEween = m_mainTEween.From(&m_ships->GetTransform()->Position.x).To(-m_selectedShip * 6.0f).Time(0.4f)
				.OnFinish([this]() {this->EnableInput(); });

			m_mainTEween = m_mainTEween.From(&m_panel->GetTransform()->Position.x)
				.To(600.0f * GameManager::GetInstance().GetDesignScale()).Time(0.4f).Easing(ET_BACK_OUT);
			AudioManager::GetInstance().PlaySoundEffect("Resource/SoundEffect/select09.mp3");
			
			ElixirLog("SelectedShip: " + std::to_string(m_selectedShip));
		}
		
		if (GetAsyncKeyState('E') & 0x8000 && m_selectedShip < m_maxShips - 1 && m_selectionNumber == 0)
		{
			m_inputEnabled = false;
			m_selectedShip++;

			m_mainTEween = m_mainTEween.From(&m_ships->GetTransform()->Position.x).To(-m_selectedShip * 6.0f).Time(0.4f)
				.OnFinish([this]() {this->EnableInput(); });

			m_mainTEween = m_mainTEween.From(&m_panel->GetTransform()->Position.x)
				.To(600.0f * GameManager::GetInstance().GetDesignScale()).Time(0.4f).Easing(ET_BACK_OUT);
			AudioManager::GetInstance().PlaySoundEffect("Resource/SoundEffect/select09.mp3");
			ElixirLog("SelectedShip: " + std::to_string(m_selectedShip));
		}

		if (GetAsyncKeyState('O') & 0x8000 && m_selectedShip > 0 && m_selectionNumber == 1)
		{
			m_inputEnabled = false;
			m_selectedShip--;
			m_mainTEween = m_mainTEween.From(&m_ships->GetTransform()->Position.x).To(-m_selectedShip * 6.0f).Time(0.4f)
				.OnFinish([this]() {this->EnableInput(); });

			m_mainTEween = m_mainTEween.From(&m_panel->GetTransform()->Position.x)
				.To(600.0f * GameManager::GetInstance().GetDesignScale()).Time(0.4f).Easing(ET_BACK_OUT);

			AudioManager::GetInstance().PlaySoundEffect("Resource/SoundEffect/select09.mp3");
			ElixirLog("SelectedShip: " + std::to_string(m_selectedShip));
		}

		if (GetAsyncKeyState('I') & 0x8000 && m_selectedShip < m_maxShips - 1 && m_selectionNumber == 1)
		{
			m_inputEnabled = false;
			m_selectedShip++;

			m_mainTEween = m_mainTEween.From(&m_ships->GetTransform()->Position.x).To(-m_selectedShip * 6.0f).Time(0.4f)
				.OnFinish([this]() {this->EnableInput(); });

			m_mainTEween = m_mainTEween.From(&m_panel->GetTransform()->Position.x)
				.To(600.0f * GameManager::GetInstance().GetDesignScale()).Time(0.4f).Easing(ET_BACK_OUT);

			ElixirLog("SelectedShip: " + std::to_string(m_selectedShip));
		}
#endif
	}

	m_selectedPlanet->GetTransform()->Rotation.y += 30.0f * dt;
	m_mainTEween.Update(dt);
}

void MachineSelect::SetImage()
{
	Manager->GetPackage()->LoadPackage("Packages/MachineScene.pkg");

	auto playerLabel = Manager->GetCurrentScene()->GetObjectByName("PlayerLabel");

	auto playerNum = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	playerNum->GetTransform()->Position = playerLabel->GetTransform()->Position;
	playerNum->GetTransform()->Scale = playerLabel->GetTransform()->Scale;
	playerNum->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/MachineScene/" + std::to_wstring(m_selectionNumber + 1) + L".png");
	playerNum->GetTransform()->Position.x += 200.0f;

	m_selectedPlanet = Manager->GetCurrentScene()->GetObjectByName("SelectedPlanet");

	int planetIndex = ENote::GetInstance().Notify<int>("GetSelectedSong");
	m_selectedPlanet->GetRenderer()->Material.roughness = Manager->GetTextureManager()->AddTexture(L"Resource/balls/200.png");
	m_selectedPlanet->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(s2ws(CourseDataArray[planetIndex].planetSkinPath));

	//画像表示　位置、サイズ
	m_title = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_title->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/ship_sel_title.png");
	m_title->GetTransform()->Position = Vec3f(0, -410, 0);
	m_title->GetTransform()->Scale = Vec3f(0.5f, 0.5f, 0);

	m_panel = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_panel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/MachineScene/ship_panel.png");
	m_panel->GetTransform()->Position = Vec3f(900, 0, 0);
	m_panel->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/back_button.png");
	back->GetTransform()->Position = Vec3f(-500, -300, 0);
	back->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	auto next = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	next->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/next_button.png");
	next->GetTransform()->Position = Vec3f(500, -300, 0);
	next->GetTransform()->Scale = Vec3f(0.7f, 0.7f, 0);

	m_ships = Manager->GetCurrentScene()->CreateObject(OBJECT_TRANSFORM);

	for (int i = 0; i < m_maxShips; ++i)
	{
		auto ship = Manager->GetPackage()->LoadPackage(MachineDataArray[i].path)[0];
		ship->GetTransform()->Position = Vec3f(i * 6.0f, -0.3f, 6.0f);
		ship->GetTransform()->Rotation.y = 30.0f;
		ship->GetTransform()->Rotation.x = -14.0f;
		m_ships->AddChild(ship);
	}
}

void MachineSelect::BlackImage()
{
	m_back = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
	m_back->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
	m_back->GetTransform()->Position = Vec3f(0, 0, 0);
	m_back->GetTransform()->Scale = Vec3f(150, 100, 0);
}

void MachineSelect::StartAnim()
{
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(-2300.0f).Time(0.7f);
}

void MachineSelect::BackAnim()
{
	m_back->GetTransform()->Position.x = 2000;
	m_mainTEween = m_mainTEween.From(&m_back->GetTransform()->Position.x).To(0.0f).Time(0.7f);
}

void MachineSelect::ChangeScene()
{
	if (m_twoPlayerMode && m_selectionNumber == 0)
	{
		m_selectionNumber++;
		m_ship01 = m_selectedShip;
		Manager->ChangeScene("MachineSelect");
	}
	else
	{
		AudioManager::GetInstance().StopAllMusic();
		if (m_selectionNumber == 0)
			m_ship01 = m_selectedShip;
		else
			m_ship02 = m_selectedShip;
		m_selectionNumber = 0;
		Manager->ChangeScene("PulseGame");
	}
	
}

void MachineSelect::BackScene()
{
	if (m_twoPlayerMode && m_selectionNumber > 0)
	{
		m_selectionNumber--;
		Manager->ChangeScene("MachineSelect");
	}
	else
	{
		m_selectionNumber = 0;
		Manager->ChangeScene("SongSelect");
	}
	
}

void MachineSelect::EnableInput()
{
	m_inputEnabled = true;
}

int MachineSelect::GetPlayer1Ship()
{
	return m_ship01;
}

int MachineSelect::GetPlayer2Ship()
{
	return m_ship02;
}

