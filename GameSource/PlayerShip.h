#pragma once
#include "../Source/System/IElixirGame.h"
#include <vector>
#include "../Source/Helper/TypeHelper.h"
#include "../Source/Graphics/CameraManager.h"
#include "CommandInterpreter.h"

class PlayerShip
{
public:
	void Initialize(Elixir::SceneManager* sceneManager, std::vector<Elixir::Vec3f> line, float radius, int playerNum);

	void UpdateShipPos(float dt);

	void SetPlayerPos(float dt);

	float GetPlayerRot();

private:
	void LateInit();

	std::vector<Elixir::Vec3f> m_lineData;
	float m_currentPos;
	float m_aheadPos;
	float m_fov;
	int m_aheadIndex;
	int m_PlayerNum;
	int m_shipNum;

	int m_currentIndex;
	Elixir::GameObject* m_player;
	Elixir::GameObject* m_opponent;
	Elixir::GameObject* m_col1;
	Elixir::GameObject* m_col2;
	Elixir::GameObject* m_meterNeedle;
	Elixir::GameObject* m_meterFrame;
	Elixir::GameObject* m_playerArrow;
	Elixir::GameObject* m_playerRankNumber;

	int m_colIndex;
	bool m_colHasDetection;
	bool m_hasCollided;
	bool m_didLateInit;
	Camera* m_camera;
	//The target to look at
	Elixir::Vec3f m_target;

	float m_rotationAngle;
	float m_travelSpeed;
	float m_rotationSpeed;
	float m_obstaclePenalty;
	float m_slipStreamBonus;
	float m_pathRadius;

	float m_cameraRadius;
	float m_cameraZDistance;

	int m_currentCombo;
	int m_timingBouns;
	int m_enemyShipNum;
	Elixir::Vec3f m_upVec;
	std::vector<Elixir::GameObject*>m_ObstacleList;
	//For convinience
	Elixir::SceneManager* Manager;
};