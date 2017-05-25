#pragma once

#include"RhythmManager.h"
#include "PlayerShip.h"
#include "CommandInterpreter.h"


class AI
{
public:
	~AI();

	void Initialize(Elixir::SceneManager* sceneManager,int playerNum,PlayerShip* pship,RhythmManager* rhythm);
	void Update(float dt);

private:

	enum HitNumber 
	{
		HN_GREAT = 8,
		HN_GOOD = 3,
		HN_BAD = 1
	};

	int m_HitDiff[3][3]
	{
		///{GREAT,GOOD,BAD}
		{85,60,20},	//Easy
		{75,40,10 },//Normal
		{65,20,5}//Hard
	};


	int m_playerNum;
	int m_TimingCount = 0;
	int m_randNum;
	int m_diff;
	bool m_musicFinish = false;

	void RhythmHitCheck();

	std::vector<int>m_NotesLaneNumber;
	std::vector<double>m_NotesTiming;

	RhythmManager* m_rhythm;
	PlayerShip* m_ship;

};

