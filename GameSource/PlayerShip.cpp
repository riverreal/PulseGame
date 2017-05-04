#include "PlayerShip.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/MathHelper.h"
#include "../Source/Helper/ENote.h"

using namespace Elixir;

void PlayerShip::Initialize(SceneManager * sceneManager, std::vector<Vec3f> line, float radius)
{
	Manager = sceneManager;
	m_currentIndex = 0;
	m_aheadIndex = 0;
	m_travelSpeed = 0.1f;
	m_lineData = line;
	m_rotationAngle = 0.0f;
	m_rotationSpeed = 1.3f;
	m_pathRadius = radius + 2.5f;
	m_target = Vec3f(0.0f, 10.0f, -10.0f);
	m_currentCombo = 0;
	//ignore first and last dots
	//m_lineData.erase(m_lineData.begin());
	//m_lineData.pop_back();

	m_currentPos = 0.5f;
	m_aheadPos = 0.7f;
	
	m_upVec = Vec3f(1.0f, 0.0f, 0.0f);

	m_dummyBall = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);
	m_dummyBall->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);


	m_player = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);
	m_player->GetRenderer()->Model = Manager->GetModel()->AddModelFromFile("Resource/ship.obj");
	m_player->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/blue.jpg");
	//m_player->GetTransform()->Scale = Vec3f(0.01f);
	m_player->GetTransform()->Position = MathHelper::GetPointInCMSpline(m_lineData[0], m_lineData[1], m_lineData[2], m_lineData[3], m_currentPos).Position;
	m_target = MathHelper::GetPointInCMSpline(m_lineData[0], m_lineData[1], m_lineData[2], m_lineData[3], m_aheadPos).Position;
}

void PlayerShip::UpdateShipPos(float dt)
{
	m_currentPos += m_travelSpeed * dt;
	m_aheadPos += m_travelSpeed * dt;

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
	
	auto aheadPoint = MathHelper::GetPointInCMSpline(m_lineData[m_aheadIndex], m_lineData[m_aheadIndex + 1], m_lineData[m_aheadIndex + 2], m_lineData[m_aheadIndex+ 3], m_aheadPos);

	m_target = aheadPoint.Position;
	//Vec3f aimDir = (m_dummyBall->GetTransform()->Position - m_player->GetTransform()->Position).FastNormalize();
	//auto q = m_player->GetTransform()->Position.QuaternionLookRotation(aimDir, Vec3f(0.0f, 1.0f, 0.0f));
	m_dummyBall->GetTransform()->Position = m_target;

	SetPlayerPos(dt);

	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_rotationAngle += 1.3f * dt;
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_rotationAngle-= 1.3f * dt;
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

	m_player->GetTransform()->Position = cmPoint.Position + circlePos;

	Vec3f aimDir = (m_dummyBall->GetTransform()->Position - m_player->GetTransform()->Position).FastNormalize();
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
}
