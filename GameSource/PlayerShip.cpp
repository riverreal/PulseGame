#include "PlayerShip.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

void PlayerShip::Initialize(SceneManager * sceneManager, std::vector<Vec3f> line)
{
	Manager = sceneManager;
	m_currentIndex = 0;
	m_aheadIndex = 0;
	m_travelSpeed = 0.4f;
	m_lineData = line;
	m_rotationAngle = 0.0f;
	m_target = Vec3f(0.0f, 10.0f, -10.0f);
	//ignore first and last dots
	//m_lineData.erase(m_lineData.begin());
	//m_lineData.pop_back();

	m_currentPos = 0.5f;
	m_aheadPos = 0.6f;
	
	m_upVec = Vec3f(1.0f, 0.0f, 0.0f);

	//m_dummyBall = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);
	//m_dummyBall->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);


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
			m_aheadPos = 0.6f;
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

	auto cmPoint = MathHelper::GetPointInCMSpline(m_lineData[m_currentIndex], m_lineData[m_currentIndex + 1], m_lineData[m_currentIndex + 2], m_lineData[m_currentIndex + 3], m_currentPos);
	auto aheadPoint = MathHelper::GetPointInCMSpline(m_lineData[m_aheadIndex], m_lineData[m_aheadIndex + 1], m_lineData[m_aheadIndex + 2], m_lineData[m_aheadIndex+ 3], m_aheadPos);

	m_target = aheadPoint.Position;

	//update player pos
	//m_player->GetTransform()->Position = cmPoint.Position;
	Vec3f camPos = Manager->GetCurrentScene()->GetCamera()->GetPosition();
	//Vec3f aimDir = (m_dummyBall->GetTransform()->Position - m_player->GetTransform()->Position).FastNormalize();
	//Log() << aimDir.x << ", " << aimDir.y << ", " << aimDir.z << "\n";
	//auto q = m_player->GetTransform()->Position.QuaternionLookRotation(aimDir, Vec3f(0.0f, 1.0f, 0.0f));

	//m_dummyBall->GetTransform()->Position = m_target;

	//m_player->GetTransform()->Rotation = MathHelper::Quaternion2Euler(q) * (180 / DirectX::XM_PI);
	//m_player->GetTransform()->Rotation.x = 180;
	//m_player->GetTransform()->Rotation.z = abs(m_player->GetTransform()->Rotation.z);
}
