#pragma once

#include "../Source/System/IElixirGame.h"
#include <vector>
#include "PlayerShip.h"
#include "RhythmManager.h"

using namespace Elixir;

class PulseGame : public IElixirGame
{
public:
	void InitTestScene();
	void UpdateTestScene(float dt);

	virtual void Init() override;
	virtual void Update(float dt) override;

private:
	std::vector<Vec3f> m_lineData;
	PlayerShip m_player;
	RhythmManager m_rhythm;
	bool m_pause;
};