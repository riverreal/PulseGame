#pragma once

#include "../Source/System/IElixirGame.h"
#include"PulseGame.h"
#include "../Source/Helper/ETween.h"

using namespace Elixir;

class MachineSelect : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	void BlackImage();
	void StartAnim();
	void BackAnim();
	void ChangeScene();
	void BackScene();
	void EnableInput();

	int GetPlayer1Ship();
	int GetPlayer2Ship();

private:
	bool m_inputEnabled;
	bool m_twoPlayerMode;
	PulseGame m_pulse;
	ETween<float> m_mainTEween;
	GameObject * m_title;
	GameObject * m_panel;
	GameObject * m_back;
	GameObject* m_selectedPlanet;
	GameObject* m_ships;
	int m_maxShips;
	int m_selectedShip;
	int m_selectionNumber;
	int m_ship01;
	int m_ship02;
};