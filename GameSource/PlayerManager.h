#pragma once

#include "RhythmManager.h"
#include "PlayerShip.h"
#include <vector>

class PlayerManager
{
public:
	void AddPlayer();
	void AddAI();

	struct PlayerComponents
	{
		RhythmManager rythm;
		PlayerShip ship;
	};

private:
	std::vector<PlayerComponents> m_players;
};