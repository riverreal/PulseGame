#include "PlayerShip.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/MathHelper.h"
#include "../Source/Helper/ENote.h"
#include"../External Soruce/cpplinq.hpp"
#include "Hitjudgment.h"

using namespace Elixir;
using namespace cpplinq;
void PlayerShip::Initialize(SceneManager * sceneManager, std::vector<Vec3f> line, float radius, int playerNum)
{
	Manager = sceneManager;
	m_currentIndex = 0;
	m_aheadIndex = 0;
	m_travelSpeed = 0.1f;
	m_lineData = line;
	m_rotationAngle = 0.0f;
	m_rotationSpeed = 1.3f;
	m_cameraRadius = radius + 1.0f;
	m_cameraZDistance = 1.3f;
	m_pathRadius = radius + 0.5f;
	m_target = Vec3f(0.0f, 10.0f, -10.0f);
	m_currentCombo = 0;
	m_currentPos = 0.5f;
	m_aheadPos = 0.7f;
	m_colIndex = 0;
	m_timingBouns = 0;
	m_upVec = Vec3f(1.0f, 0.0f, 0.0f);
	m_colHasDetection = false;

	m_camera = Manager->GetCurrentScene()->GetCamera();
	m_camera->SetPosition(0.0f, 0.0f, 10.0f);

	m_PlayerNum = playerNum;

	m_player = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);
	m_player->GetRenderer()->Model = Manager->GetModel()->AddModelFromFile("Resource/ships/shipImp.obj");
	m_player->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/ships/shipAlbedoEm.png");
	m_player->GetRenderer()->Material.metallic = Manager->GetTextureManager()->AddTexture(L"Resource/ships/shipMetallic.png");
	m_player->GetRenderer()->Material.normal = Manager->GetTextureManager()->AddTexture(L"Resource/ships/shipNormal.png");
	m_player->GetRenderer()->Material.emissive = Manager->GetTextureManager()->AddTexture(L"Resource/ships/shipEmissive.png");
	m_player->GetRenderer()->Material.roughness = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/75.png");
	m_player->GetTransform()->Scale = Vec3f(0.1f);
	m_player->GetTransform()->Position = MathHelper::GetPointInCMSpline(m_lineData[0], m_lineData[1], m_lineData[2], m_lineData[3], m_currentPos).Position;
	m_target = MathHelper::GetPointInCMSpline(m_lineData[0], m_lineData[1], m_lineData[2], m_lineData[3], m_aheadPos).Position;

	m_ObstacleList = Manager->LoadScene("Resource/test8couse.escene");
	m_ObstacleList = from(m_ObstacleList)
		>> orderby_ascending([](GameObject* obj) {return obj->GetTransform()->Position.Length(); })
		>> to_vector();

	for (auto &o : m_ObstacleList)
	{
		o->SetName("ok");
	}
	m_col1 = new GameObject();
	m_col1->AddComponent<Transform>();
	m_col1->AddComponent<Renderer3D>();
	m_col1->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);
	m_col1->GetTransform()->Position.z = 0.15f;
	m_col1->GetTransform()->Scale = Vec3f(0.3f);
	//m_player->AddChild(m_col1);

	m_col2 = Manager->GetCurrentScene()->CreateObject(OBJECT_RENDER);
	m_col2->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);
	//m_col2->GetTransform()->Position.z = -1.28f;
	//m_col2->GetTransform()->Position.y = -m_pathRadius;
	m_col2->GetTransform()->Scale = Vec3f(5
		
		
		.0f);
	//m_col2->GetRenderer()->Enabled = false;
	//m_player->AddChild(m_col2);
	m_col2->SetName("Col2");
}
void PlayerShip::UpdateShipPos(float dt)
{
	if (Hitjudgment::SpColliding(m_ObstacleList[m_colIndex], m_col2))
	{
		m_ObstacleList[m_colIndex]->GetRenderer()->Material.emissive = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/255.png");
	}
	else
	{
		m_ObstacleList[m_colIndex]->GetRenderer()->Material.emissive = Manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/0.png");
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
			//m_colIndex++;
		}
	}

	Log() << m_colIndex << "\n";
	/*
	auto speed = m_travelSpeed * dt +(m_currentCombo + m_timingBouns)* dt *0.001f;
	m_currentPos+= speed;
	m_aheadPos += speed;

	if (m_currentPos >= 1.0f)
	{
		m_currentPos -= 0.5f;
		m_currentIndex++;

		if (m_currentIndex + 3 >= m_lineData.size())
		{
			m_currentIndex = 0;
			m_aheadIndex = 0;
			m_currentPos = 0.5f;
			m_aheadPos = 0.7f;
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

	m_currentCombo = ENote::GetInstance().Notify<int>("GetCombo");
	m_timingBouns = ENote::GetInstance().Notify<int>("GetTimingBonus");


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
	*/
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

	Vec3f pc2(cos(rotationAngle) * m_cameraRadius, -m_cameraZDistance, sin(rotationAngle) * m_cameraRadius);

	Vec3f cameraCirclePos;
	cameraCirclePos.x = pc2.x * right.x + pc2.y * up.x + pc2.z * forward.x;
	cameraCirclePos.y = pc2.x * right.y + pc2.y * up.y + pc2.z * forward.y;
	cameraCirclePos.z = pc2.x * right.z + pc2.y * up.z + pc2.z * forward.z;

	DirectX::XMFLOAT3 camPos;
	camPos.x = cmPoint.Position.x + cameraCirclePos.x;
	camPos.y = cmPoint.Position.y + cameraCirclePos.y;
	camPos.z = cmPoint.Position.z + cameraCirclePos.z;

	//m_camera->SetPosition(camPos);

	DirectX::XMFLOAT3 target;
	target.x = m_target.x; target.y = m_target.y; target.z = m_target.z;

	//m_camera->SetLookAt(m_camera->GetPosition(), playerPos, upVec);//DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
}
