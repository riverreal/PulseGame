#include "PlayerManager.h"
#include "CommandInterpreter.h"
#include <Windows.h>

PlayerManager::PlayerManager()
	:m_registeredHuman(0),
	m_registeredAI(0),
	m_playerNum(0)
{}

PlayerManager::~PlayerManager()
{
	Release();
}

void PlayerManager::AddPlayer(Elixir::SceneManager* manager, int playerNum, std::vector<Elixir::Vec3f> lineData, float radius, DIFF difficulty)
{
	
	PlayerComponents pComp;
	pComp.rhythm = new RhythmManager;
	pComp.ship = new PlayerShip;
	pComp.ship->Initialize(manager, lineData, radius, playerNum);
	pComp.rhythm->Initialize(manager, difficulty, playerNum);
	m_players.push_back(pComp);
	RegistHumanPlayer();
}

void PlayerManager::AddAI(Elixir::SceneManager* manager, int playerNum, std::vector<Elixir::Vec3f> lineData, float radius, DIFF difficulty)
{
	PlayerComponents pComp;
	pComp.rhythm = new RhythmManager;
	pComp.ship = new PlayerShip;
	pComp.ship->Initialize(manager, lineData, radius, playerNum);
	pComp.rhythm->Initialize(manager, difficulty, playerNum);
	m_players.push_back(pComp);
	AI* ai = new AI;
	ai->Initialize(manager, playerNum, pComp.ship, pComp.rhythm);
	m_AI.push_back(ai);
	RegistAIPlayer();
}

void PlayerManager::Release()
{
	for (auto &ai : m_AI)
	{
		delete ai;
	}

	for (auto &comp : m_players)
	{
		delete comp.rhythm;
		delete comp.ship;
	}

	m_AI.clear();
	m_players.clear();
}

void PlayerManager::Update(float dt)
{
	//Input update
	for (int i = 0; i < m_registeredHuman; ++i)
	{
		HumanInput(i);
	}

	//AI Update
	for (auto &ai : m_AI)
	{
		ai->Update(dt);
	}

	//Component  Update
	for (auto &comp : m_players)
	{
		comp.rhythm->Update(dt);
		comp.ship->UpdateShipPos(dt);
	}
}

void PlayerManager::RegistHumanPlayer()
{
	m_registeredHuman++;
	m_playerNum++;
}

void PlayerManager::RegistAIPlayer()
{
	m_registeredAI++;
	m_playerNum++;
}

void PlayerManager::HumanInput(int playerNum)
{
	switch (playerNum)
	{
	case 0:
		if (GetAsyncKeyState('Q') & 0x8000)
			CommandInterpreter::GetInstance().setStrafeLeft(playerNum, true);
		else
			CommandInterpreter::GetInstance().setStrafeLeft(playerNum, false);

		if (GetAsyncKeyState('E') & 0x8000)
			CommandInterpreter::GetInstance().setStrafeRight(playerNum, true);
		else
			CommandInterpreter::GetInstance().setStrafeRight(playerNum, false);

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			CommandInterpreter::GetInstance().setRightNotes(playerNum, true);
		else
			CommandInterpreter::GetInstance().setRightNotes(playerNum, false);

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			CommandInterpreter::GetInstance().setCenterNotes(playerNum, true);
		else
			CommandInterpreter::GetInstance().setCenterNotes(playerNum, false);

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			CommandInterpreter::GetInstance().setLeftNotes(playerNum, true);
		else
			CommandInterpreter::GetInstance().setLeftNotes(playerNum, false);
		break;

	case 1:
		if (GetAsyncKeyState('O') & 0x8000)
			CommandInterpreter::GetInstance().setStrafeLeft(playerNum, true);
		else
			CommandInterpreter::GetInstance().setStrafeLeft(playerNum, false);

		if (GetAsyncKeyState('I') & 0x8000)
			CommandInterpreter::GetInstance().setStrafeRight(playerNum, true);
		else
			CommandInterpreter::GetInstance().setStrafeRight(playerNum, false);

		if (GetAsyncKeyState('M') & 0x8000)
			CommandInterpreter::GetInstance().setRightNotes(playerNum, true);
		else
			CommandInterpreter::GetInstance().setRightNotes(playerNum, false);

		if (GetAsyncKeyState('N') & 0x8000)
			CommandInterpreter::GetInstance().setCenterNotes(playerNum, true);
		else
			CommandInterpreter::GetInstance().setCenterNotes(playerNum, false);

		if (GetAsyncKeyState('B') & 0x8000)
			CommandInterpreter::GetInstance().setLeftNotes(playerNum, true);
		else
			CommandInterpreter::GetInstance().setLeftNotes(playerNum, false);
		break;

	default:
		break;
	}
}