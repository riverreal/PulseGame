#pragma once

#include "../Source/System/IElixirGame.h"
#include <vector>
#include "../Source/Helper/TypeHelper.h"
#include "../Source/Graphics/CameraManager.h"

class PlayerShip
{
public:
	void Initialize(Elixir::SceneManager* sceneManager, std::vector<Elixir::Vec3f> line, float radius);

	void UpdateShipPos(float dt);

	void SetPlayerPos(float dt);

private:
	std::vector<Elixir::Vec3f> m_lineData;
	float m_currentPos;
	float m_aheadPos;
	int m_aheadIndex;

	int m_currentIndex;
	Elixir::GameObject* m_player;
	Camera* m_camera;
	//The target to look at
	Elixir::Vec3f m_target;

	float m_rotationAngle;
	float m_travelSpeed;
	float m_rotationSpeed;
	float m_pathRadius;

	float m_cameraRadius;
	float m_cameraZDistance;

	int m_currentCombo;

	Elixir::Vec3f m_upVec;

	//For convinience
	Elixir::SceneManager* Manager;
};