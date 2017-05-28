#pragma once

#include "../Source/System/IElixirGame.h"
#include <vector>
#include "PlayerShip.h"
#include "../Source/Helper/ETween.h"
#include "RhythmManager.h"
#include"ResultScene.h"

using namespace Elixir;

class PulseGame : public IElixirGame
{
public:
	void InitTestScene();
	void UpdateTestScene(float dt);

	virtual void Init() override;
	virtual void Update(float dt) override;
	void BlackImage();
	void StartAnim();
	void BackAnim();
	void ChangeScene();

private:
	std::vector<Vec3f> m_lineData;
	PlayerShip m_player;
	RhythmManager m_rhythm;
	bool m_pause;
	ETween<float> m_mainTEween;
	ResultScene m_result;
	GameObject * m_back;
};