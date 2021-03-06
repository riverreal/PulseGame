#include "PlayerShip.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/MathHelper.h"
#include "../Source/Helper/ENote.h"
#include"../External Soruce/cpplinq.hpp"
#include "Hitjudgment.h"
#include "../Source/Helper/ENote.h"
#include "CourseSelect.h"
#include <stdlib.h>

using namespace Elixir;
using namespace cpplinq;

void PlayerShip::Initialize(SceneManager * sceneManager, std::vector<Vec3f> line, float radius, int playerNum)
{
	Manager = sceneManager;
	m_currentIndex = 0;
	m_aheadIndex = 0;
	m_slipStreamBonus = 0;
	m_obstaclePenalty = 100.0f;
	m_travelSpeed = 0.08f;
	m_lineData = line;
	m_rotationAngle = playerNum * XM_PI / 6;
	m_didLateInit = false;
	//max course index
	m_progressValue = 0.0f;

	m_rotationSpeed = 1.0f;
	m_cameraRadius = radius + 0.8f;
	m_cameraZDistance = 1.0f;
	m_pathRadius = radius + 0.5f;
	m_target = Vec3f(0.0f, 10.0f, -10.0f);
	m_currentCombo = 0;
	m_currentPos = 0.5f;
	m_aheadPos = 0.7f;
	m_colIndex = 0;
	m_timingBouns = 0;
	m_upVec = Vec3f(1.0f, 0.0f, 0.0f);
	m_colHasDetection = false;
	m_hasCollided = false;
	m_fov = 0.8f;

	if (playerNum != 2)
	{
		bool isSplit = ENote::GetInstance().Notify<bool>("GetSplitScreen");
		m_playerRankNumber = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
		m_playerRankNumber->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/PulseGameScene/first.png");
		m_playerRankNumber->GetTransform()->Scale = Vec3f(0.8f, 0.8f, 0.8f);
		m_playerRankNumber->GetTransform()->Position = Vec3f(850, -400, 0);
		m_playerRankNumber->SetName("RankSprite");
		if (isSplit)
		{
			auto screenW = GameManager::GetInstance().GetScreenWidth();
			auto halfScreenW = screenW * 0.5;

			if (playerNum == 0)
			{
				m_playerRankNumber->GetTransform()->Position.x = -100;
			}
		}
	}
	
	int pNumIndex = playerNum == 0 ? 0 : 1;
	ENote::GetInstance().AddNote<float>("GetPlayerRot" + pNumIndex, [this]() {return this->GetPlayerRot(); });
	ENote::GetInstance().AddNote<float>("GetPlayerProgress" + std::to_string(pNumIndex), [this]() {return this->GetProgressValue(); });

	if (playerNum == 0)
		m_camera = Manager->GetCurrentScene()->GetCamera();
	else
		m_camera = Manager->GetCurrentScene()->GetSecCamera();

	m_camera->SetPosition(0.0f, 0.0f, 10.0f);

	m_PlayerNum = playerNum;
	if (playerNum == 2)
	{
		//AI
		int randomShip = rand() % 3;
		m_shipNum = randomShip;
		auto shipPkg = Manager->GetPackage()->LoadPackage(MachineDataArray[randomShip].path);
		m_player = shipPkg[0];

		auto shipEffect = Manager->GetPackage()->LoadPackage("Packages/ships/effect0" + std::to_string(randomShip + 1) + ".pkg");
		m_shipEffect01 = shipEffect[0];
		m_shipEffect02 = shipEffect[1];
		m_player->AddChild(m_shipEffect01);
		m_player->AddChild(m_shipEffect02);
	}
	else
	{
		//Player
		std::string note = "GetPlayer" + std::to_string(playerNum + 1) + "Ship";
		m_shipNum = ENote::GetInstance().Notify<int>(note);
		auto shipPkg = Manager->GetPackage()->LoadPackage(MachineDataArray[m_shipNum].path);
		m_player = shipPkg[0];

		auto shipEffect = Manager->GetPackage()->LoadPackage("Packages/ships/effect0" + std::to_string(m_shipNum + 1) + ".pkg");
		m_shipEffect01 = shipEffect[0];
		m_shipEffect02 = shipEffect[1];
		m_player->AddChild(m_shipEffect01);
		m_player->AddChild(m_shipEffect02);
	}

	m_shipEffect01->GetTransform()->Position = m_shipEffect01->GetTransform()->Position / m_player->GetTransform()->Scale.x;
	m_shipEffect02->GetTransform()->Position = m_shipEffect02->GetTransform()->Position / m_player->GetTransform()->Scale.x;
	m_shipEffect01->GetTransform()->Scale = m_shipEffect01->GetTransform()->Scale / m_player->GetTransform()->Scale.x;
	m_shipEffect02->GetTransform()->Scale = m_shipEffect02->GetTransform()->Scale / m_player->GetTransform()->Scale.x;
	ShipEffectAnim(true);

	if (m_PlayerNum != 0)
	{
		ENote::GetInstance().AddNote<int>("GetPlayerNumber", [this] {return m_PlayerNum;});
	}
	
	m_player->SetName("Ship" + pNumIndex);
	m_player->GetTransform()->Scale = m_player->GetTransform()->Scale * 0.2f;

	m_player->GetTransform()->Position = MathHelper::GetPointInCMSpline(m_lineData[0], m_lineData[1], m_lineData[2], m_lineData[3], m_currentPos).Position;
	m_target = MathHelper::GetPointInCMSpline(m_lineData[0], m_lineData[1], m_lineData[2], m_lineData[3], m_aheadPos).Position;
	
	m_ObstacleList = Manager->GetPackage()->LoadPackage("Packages/ingame/Obstacle/Obstacle01.pkg");
	m_ObstacleList = from(m_ObstacleList)
		>> orderby_ascending([](GameObject* obj) {return obj->GetTransform()->Position.Length(); })
		>> to_vector();

	m_col1 = new GameObject();
	m_col1->AddComponent<Transform>();
	m_col1->AddComponent<Renderer3D>();
	m_col1->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);
	m_col1->GetTransform()->Position.z = 0.15f;
	m_col1->GetTransform()->Scale = Vec3f(0.3f);
	m_col1->GetRenderer()->Enabled = false;
	m_player->AddChild(m_col1);

	m_col2 = Manager->GetCurrentScene()->CreateObject(OBJECT_RENDER);
	m_col2->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);
	m_col2->GetTransform()->Position.z = -1.28f;
	m_col2->GetTransform()->Position.y = -m_pathRadius;
	m_col2->GetTransform()->Scale = Vec3f(5.0f);
	m_col2->GetRenderer()->Enabled = false;
	m_player->AddChild(m_col2);

	if (m_PlayerNum != 2)
	{
		auto meters = Manager->GetPackage()->LoadPackage("Packages/ingame/Meter.pkg");
		m_meterFrame = meters[0];
		m_meterNeedle = meters[1];
		m_player->AddChild(m_meterFrame);
		m_player->AddChild(m_meterNeedle);
		m_meterFrame->GetTransform()->Position.z += 0.3f / m_player->GetTransform()->Scale.x;
		m_meterNeedle->GetTransform()->Position.z += 0.301f / m_player->GetTransform()->Scale.x;
		m_meterNeedle->GetTransform()->Rotation.x = 90.f;
		m_meterFrame->GetTransform()->Position.x += 0.1f / m_player->GetTransform()->Scale.x;
		m_meterNeedle->GetTransform()->Position.x += 0.1f / m_player->GetTransform()->Scale.x;
		m_meterFrame->GetTransform()->Position.y += 0.05f / m_player->GetTransform()->Scale.x;
		m_meterNeedle->GetTransform()->Position.y += 0.05f / m_player->GetTransform()->Scale.x;
		m_meterFrame->GetTransform()->Scale = m_meterFrame->GetTransform()->Scale / m_player->GetTransform()->Scale.x;
		m_meterNeedle->GetTransform()->Scale = m_meterNeedle->GetTransform()->Scale / m_player->GetTransform()->Scale.x;
		m_meterNeedle->GetTransform()->Scale = m_meterNeedle->GetTransform()->Scale / 15;
		m_meterFrame->GetTransform()->Scale = m_meterFrame->GetTransform()->Scale / 15;

		m_playerArrow = Manager->GetCurrentScene()->CreateObject(OBJECT_RENDER);
		m_playerArrow->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_PLAIN);
		m_playerArrow->GetRenderer()->CastShadow = false;
		m_playerArrow->GetRenderer()->EnableBackFaceCulling = false;
		m_playerArrow->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/PulseGameScene/playerPos.png");
		m_playerArrow->GetRenderer()->Material.emissive = Manager->GetTextureManager()->AddTexture(L"Resource/balls/25.png");
		m_playerArrow->GetTransform()->Rotation = Vec3f(-90, 90, 90);
		m_player->AddChild(m_playerArrow);
		m_playerArrow->GetTransform()->Scale = m_playerArrow->GetTransform()->Scale / m_player->GetTransform()->Scale.x;
		m_playerArrow->GetTransform()->Position.y += 2.2f / m_player->GetTransform()->Scale.x;
	}
}
void PlayerShip::UpdateShipPos(float dt)
{
	LateInit();

	if (Hitjudgment::SpColliding(m_ObstacleList[m_colIndex], m_col1))
	{
		if (!m_hasCollided)
		{
			m_hasCollided = true;


			m_obstaclePenalty += 100.0f;

			//Hit process goes here!
			ElixirLog("Collided!");
		}
	}
	else
	{
		if (m_hasCollided)
		{
			m_hasCollided = false;
		}
	}


	if (Hitjudgment::SpColliding(m_ObstacleList[m_colIndex], m_col2))
	{
		m_colHasDetection = true;
	}
	else
	{
		if (m_colHasDetection)
		{
			m_colHasDetection = false;
			m_colIndex++;

			m_colIndex = MathHelper::clamp(m_colIndex, 0, m_ObstacleList.size()-1);
		}
	}

	int oponentIndex = m_PlayerNum == 0 ? 1 : 0;
	float oponentRot = ENote::GetInstance().Notify<float>("GetPlayerRot" + oponentIndex);

	//slip stream!
	if (oponentRot + 0.4f > m_rotationAngle && oponentRot - 0.4f < m_rotationAngle)
	{
		//if behind
		float delta = m_player->GetTransform()->Position.Length() - m_opponent->GetTransform()->Position.Length();

		if (delta < 0)
		{
			m_fov += 0.5f * dt;
			if (m_fov > 1.2f)
			{
				m_fov = 1.2f;
			}
			else
			{
				m_camera->SetFOV(m_fov);
				m_camera->BuildProjection();
			}
			
			m_slipStreamBonus = 510.0f * dt;
		}
	}
	else
	{
		if (m_slipStreamBonus > 0 || m_fov > 0.8f)
		{
			m_fov -= 0.5f * dt;

			if (m_fov < 0.8f)
			{
				m_fov = 0.8f;
			}
			else
			{
				m_camera->SetFOV(m_fov);
				m_camera->BuildProjection();
			}

			m_slipStreamBonus -= 1.0f * dt;
			m_slipStreamBonus = MathHelper::Max(m_slipStreamBonus, 0.0f);
		}
	}

	auto bonusSpeed = (m_currentCombo + m_timingBouns - m_obstaclePenalty + m_slipStreamBonus)* dt * 0.002f;
	auto speed = m_travelSpeed * dt + bonusSpeed;

	if (m_obstaclePenalty > 0)
	{
		m_obstaclePenalty -= 100.0f * dt;
		if (m_obstaclePenalty < 0)
		{
			m_obstaclePenalty = 0;
		}
	}

	if (m_PlayerNum != 2)
	{
		m_meterNeedle->GetTransform()->Rotation.y = MathHelper::clamp(-33.3f + bonusSpeed * 100000.0f, -33.3f, 207.f);
	}

	m_currentPos += speed;
	m_progressValue += speed;
	m_aheadPos += speed;

	if (m_currentPos >= 1.0f)
	{
		m_currentPos -= 0.5f;
		m_currentIndex++;

		/*
		if (m_currentIndex + 3 >= m_lineData.size())
		{
			m_currentIndex = 0;
			m_aheadIndex = 0;
			m_currentPos = 0.5f;
			m_aheadPos = 0.7f;
		}
		*/
		
		//setVolume
		if (m_currentIndex + 3 >= m_lineData.size() - 5 && m_player->GetTransform()->Position.Length() > m_opponent->GetTransform()->Position.Length())
		{
			AudioManager::GetInstance().GetControlledMusic()->setVolume(AudioManager::GetInstance().GetControlledMusic()->getVolume() - (1.0 / 4));
		}

		if (m_currentIndex + 3 >= m_lineData.size() - 2)
		{
			if (ENote::GetInstance().Notify<int>("GetPlayerNumber") != m_PlayerNum)
			{
				//Enemy
				std::string note = "GetPlayer" + std::to_string(ENote::GetInstance().Notify<int>("GetPlayerNumber") + 1) + "Ship";
				m_enemyShipNum = ENote::GetInstance().Notify<int>(note);
				ENote::GetInstance().AddNote<std::string>("GetLoserMachine", [this] {return MachineDataArray[m_enemyShipNum].path; });
			}
			else
			{
				//Player
				std::string note = "GetPlayer1Ship";
				m_enemyShipNum = ENote::GetInstance().Notify<int>(note);
				ENote::GetInstance().AddNote<std::string>("GetLoserMachine", [this] {return MachineDataArray[m_enemyShipNum].path; });
			}

			//setMachinePath
			ENote::GetInstance().AddNote<std::string>("GetWinnerMachine", [this] {return MachineDataArray[m_shipNum].path; });
			Manager->ChangeScene("ResultScene");
		}
	}

	if (m_aheadPos >= 1.0f)
	{
		m_aheadPos -= 0.5f;
		m_aheadIndex++;

		if (m_aheadIndex + 3 >= m_lineData.size())
		{
			m_aheadIndex--;
			m_currentPos = 1.0f;
		}
	}

	m_currentCombo = ENote::GetInstance().Notify<int>("GetCombo" + m_PlayerNum);
	m_timingBouns = ENote::GetInstance().Notify<int>("GetTimingBonus" + m_PlayerNum);

	auto aheadPoint = MathHelper::GetPointInCMSpline(m_lineData[m_aheadIndex], m_lineData[m_aheadIndex + 1], m_lineData[m_aheadIndex + 2], m_lineData[m_aheadIndex+ 3], m_aheadPos);

	m_target = aheadPoint.Position;
	SetPlayerPos(dt);

	if (CommandInterpreter::GetInstance().isStrafeRight(m_PlayerNum))
	{
		m_rotationAngle += 1.3f * dt;
	}

	if (CommandInterpreter::GetInstance().isStrafeLeft(m_PlayerNum))
	{
		m_rotationAngle -= 1.3f * dt;
	}
	
	m_effectGrow.Update(dt);
	m_effectShrink.Update(dt);
	
	if (m_PlayerNum != 2)
	{

		if (m_player->GetTransform()->Position.Length() > m_opponent->GetTransform()->Position.Length())
		{
			//1
			m_playerRankNumber->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/PulseGameScene/first.png");
		}
		else
		{
			//2
			m_playerRankNumber->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/PulseGameScene/second.png");
		}
	}
}

void PlayerShip::SetPlayerPos(float dt)
{
	auto cmPoint = MathHelper::GetPointInCMSpline(m_lineData[m_currentIndex], m_lineData[m_currentIndex + 1], m_lineData[m_currentIndex + 2], m_lineData[m_currentIndex + 3], m_currentPos);

	static int maxAxis = 0;
	if (std::abs(cmPoint.Tangent.x) > std::abs(cmPoint.Tangent.y))
	{
		if (std::abs(cmPoint.Tangent.x) > std::abs(cmPoint.Tangent.z))
		{
			maxAxis = 0;
		}
		else
		{
			maxAxis = 2;
		}
	}
	else if (std::abs(cmPoint.Tangent.y) > std::abs(cmPoint.Tangent.z))
	{
		maxAxis = 1;
	}
	else
	{
		maxAxis = 2;
	}
	
	m_rotationAngle = m_rotationAngle < 0 ? 6.28319f : m_rotationAngle;
	m_rotationAngle = m_rotationAngle > 6.28319f ? 0 : m_rotationAngle;

	Vec3f up, forward, right;
	float rotationAngle = 0.0f;

	up = cmPoint.Tangent;
	right = Vec3f(0, 0, 1);
	forward = right.Cross(up);
	right = up.Cross(forward);
	rotationAngle = m_rotationAngle;

	up = up.FastNormalize();
	right = right.FastNormalize();
	forward = forward.FastNormalize();

	Vec3f pc(cos(rotationAngle) * m_pathRadius, 0, sin(rotationAngle) * m_pathRadius);
	Vec3f circlePos;
	circlePos.x = pc.x * right.x + pc.y * up.x + pc.z * forward.x;
	circlePos.y = pc.x * right.y + pc.y * up.y + pc.z * forward.y;
	circlePos.z = pc.x * right.z + pc.y * up.z + pc.z * forward.z;

	circlePos.x = pc.x * right.x + pc.y * up.x + pc.z * forward.x;
	circlePos.y = pc.x * right.y + pc.y * up.y + pc.z * forward.y;
	circlePos.z = pc.x * right.z + pc.y * up.z + pc.z * forward.z;

	m_player->GetTransform()->Position = cmPoint.Position + circlePos;

	Vec3f aimDir = (m_target - m_player->GetTransform()->Position).FastNormalize();
	auto invAim = ((cmPoint.Position - m_player->GetTransform()->Position) * -1).FastNormalize();
	invAim.z = 0;
	//aimDir.z = 0;
	auto q = m_player->GetTransform()->Position.QLookRotation(aimDir, invAim);
	auto rot = MathHelper::Quaternion2Euler(q) * (180 / DirectX::XM_PI) + 180;
	
	int option = 0;

	if (aimDir.z < 0)
	{
		option = 0;
		rot.y *= -1;
		rot.z += 180;
		rot.x = -(360 - rot.x) - 180;
	}
	else
	{
		option = 1;
		rot.z += 180;
		rot.x = -(360 - rot.x) - 180;
	}

	m_player->GetTransform()->Rotation = rot;

	auto pPos = m_player->GetTransform()->Position;

	DirectX::XMFLOAT3 playerPos, upVec;
	Vec3f deltaPos = (m_target - pPos) * 0.5f;
	playerPos.x = pPos.x + deltaPos.x; playerPos.y = pPos.y + deltaPos.y; playerPos.z = pPos.z + deltaPos.z;
	upVec.x = up.x; upVec.y = up.y; upVec.z = up.z;

	//Camera update
	if (m_PlayerNum == 0 || m_PlayerNum == 1)
	{
		Vec3f pc2(cos(rotationAngle) * m_cameraRadius, -m_cameraZDistance, sin(rotationAngle) * m_cameraRadius);

		Vec3f cameraCirclePos;
		cameraCirclePos.x = pc2.x * right.x + pc2.y * up.x + pc2.z * forward.x;
		cameraCirclePos.y = pc2.x * right.y + pc2.y * up.y + pc2.z * forward.y;
		cameraCirclePos.z = pc2.x * right.z + pc2.y * up.z + pc2.z * forward.z;

		DirectX::XMFLOAT3 camPos;
		camPos.x = cmPoint.Position.x + cameraCirclePos.x;
		camPos.y = cmPoint.Position.y + cameraCirclePos.y;
		camPos.z = cmPoint.Position.z + cameraCirclePos.z;

		m_camera->SetPosition(camPos);

		DirectX::XMFLOAT3 target;
		target.x = m_target.x; target.y = m_target.y; target.z = m_target.z;

		m_camera->SetLookAt(m_camera->GetPosition(), playerPos, upVec);
	}
}

float PlayerShip::GetPlayerRot()
{
	return m_rotationAngle;
}

void PlayerShip::ShipEffectAnim(bool grow)
{
	if (grow)
	{
		m_effectGrow = m_effectGrow.From(&m_shipEffect01->GetTransform()->Scale.z).To(m_shipEffect01->GetTransform()->Scale.z * 2.5f).Time(0.19f);
		m_effectGrow = m_effectGrow.From(&m_shipEffect02->GetTransform()->Scale.z).To(m_shipEffect02->GetTransform()->Scale.z * 2.5f).Time(0.2f)
			.OnFinish([this]() {this->ShipEffectAnim(false); });
	}
	else
	{
		m_effectShrink = m_effectShrink.From(&m_shipEffect01->GetTransform()->Scale.z).To(m_shipEffect01->GetTransform()->Scale.z / 2.5f).Time(0.19f);
		m_effectShrink = m_effectShrink.From(&m_shipEffect02->GetTransform()->Scale.z).To(m_shipEffect02->GetTransform()->Scale.z / 2.5f).Time(0.2f)
			.OnFinish([this]() {this->ShipEffectAnim(true); });
	}
}

float PlayerShip::GetProgressValue()
{
	return m_progressValue;
}

void PlayerShip::LateInit()
{
	if (!m_didLateInit)
	{
		m_didLateInit = true;

		int opponentIndex = m_PlayerNum == 0 ? 1 : 0;
		m_opponent = Manager->GetCurrentScene()->GetObjectByName("Ship" + opponentIndex);
	}
}
