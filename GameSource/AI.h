#pragma once

#include"RhythmManager.h"
#include "PlayerShip.h"


class AI
{
public:
	AI();
	~AI();

	void Initialize(Elixir::SceneManager* sceneManager,int playerNum,PlayerShip* pship,RhythmManager* rhythm);
	void Update(float dt);

private:

	int m_playerNum;
	int timingCount = 0;

	std::vector<int>m_NotesLaneNumber;
	std::vector<double>m_NotesTiming;

};

