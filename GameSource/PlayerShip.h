#pragma once

#include "../Source/System/IElixirGame.h"
#include <vector>
#include "../Source/Helper/TypeHelper.h"

class PlayerShip
{
public:
	void Initialize(Elixir::SceneManager* sceneManager, std::vector<Elixir::Vec3f> line);

	void UpdateShipPos(float dt);

private:
	std::vector<Elixir::Vec3f> m_lineData;
	float m_currentPos;
	float m_aheadPos;
	int m_aheadIndex;

	int m_currentIndex;
	Elixir::GameObject* m_player;
	Elixir::GameObject* m_dummyBall;

	//The target to look at
	Elixir::Vec3f m_target;

	float m_rotationAngle;
	float m_travelSpeed;

	Elixir::Vec3f m_upVec;

	//For convinience
	Elixir::SceneManager* Manager;
};