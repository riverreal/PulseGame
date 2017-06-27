#pragma once

#include "../Source/System/IElixirGame.h"
#include <vector>
#include "PlayerManager.h"
#include "ResultScene.h"
#include "StageManager.h"

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
	PlayerManager m_playerManager;
	StageManager m_stageManager;
	bool m_pause;
	ETween<float> m_mainTEween;
	ResultScene m_result;
	GameObject * m_back;
};