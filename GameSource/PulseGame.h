#pragma once

#include "../Source/System/IElixirGame.h"
#include <vector>
#include "../Source/Helper/ETween.h"
#include "PlayerManager.h"

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
	PlayerManager m_playerManager;
	bool m_pause;
};