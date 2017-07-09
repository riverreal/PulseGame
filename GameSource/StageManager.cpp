#include "StageManager.h"
#include <time.h>

using namespace Elixir;

StageManager::StageManager()
{
}

void StageManager::Initialize(SceneManager* manager)
{
	m_manager = manager;

	srand(time(NULL));

	m_manager->GetCurrentScene()->SetIrradiance(m_manager->GetTextureManager()->AddTexture(L"Resource/Cubemaps/Irradiance/Irradiance.dds"));
	m_manager->GetCurrentScene()->SetEnvMap(m_manager->GetTextureManager()->AddTexture(L"Resource/Cubemaps/earth_moon_skybox.dds"));

	auto dirL = m_manager->GetCurrentScene()->GetLight()->GetModDirectionalLight();

	auto fog = Fog();
	fog.Enabled = false;
	m_manager->GetCurrentScene()->SetFog(fog);

	dirL->LightColor[0] = 1.0f;
	dirL->LightColor[1] = 1.0f;
	dirL->LightColor[2] = 1.0f;

	dirL->LightIntensity[0] = 1.0f;
	dirL->LightIntensity[1] = 0.1f;

	dirL->Direction[0] = 0.1f;
	dirL->Direction[1] = -0.6f;
	dirL->Direction[2] = -0.8f;

	m_manager->GetCurrentScene()->GetCamera()->SetPosition(0.0f, 0.0f, -10.0f);

	for (int i = 0; i < 100; i++)
	{
		CreateStarDust();
	}
}

void StageManager::CreateStarDust()
{
	auto dust = m_manager->GetCurrentScene()->CreateObject(OBJECT_RENDER);
	dust->GetRenderer()->Model = m_manager->GetModel()->AddGeometry(MODEL_TYPE_SPHERE);
	dust->GetRenderer()->Material.albedo = m_manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/255.png");
	dust->GetRenderer()->Material.emissive = m_manager->GetTextureManager()->AddTexture(L"Resources/Textures/balls/255.png");
	auto transf = dust->GetTransform();
	transf->Scale = Vec3f(0.01f);

	float x = -(rand() % 10) + 5;
	float y = (rand() % 10) - 5;
	float z = rand() % 50;

	transf->Position = Vec3f(x, y, -z);
}