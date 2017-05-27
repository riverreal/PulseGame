#pragma once

#include "RhythmManager.h"
#include "PlayerShip.h"
#include "../Source/Includes/LESystem.h"
#include "AI.h"
#include <vector>

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

	void AddPlayer(Elixir::SceneManager* manager, int playerNum, std::vector<Elixir::Vec3f> lineData, float radius, DIFF difficulty);
	void AddAI(Elixir::SceneManager* manager, int playerNum, std::vector<Elixir::Vec3f> lineData, float radius, DIFF difficulty);
	
	void Release();

	void Update(float dt);

	struct PlayerComponents
	{
		RhythmManager* rhythm;
		PlayerShip* ship;
	};

private:
	void RegistHumanPlayer();
	void RegistAIPlayer();
	void HumanInput(int playerNum);

private:
	int m_registeredHuman;
	int m_registeredAI;
	int m_playerNum;
	std::vector<PlayerComponents> m_players;
	std::vector<AI*> m_AI;
};