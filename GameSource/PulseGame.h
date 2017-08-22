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
	void CalculateProgress();

private:
	std::vector<Vec3f> m_lineData;
	PlayerManager m_playerManager;
	StageManager m_stageManager;
	bool m_pause;
	ETween<float> m_mainTEween;
	ResultScene m_result;
	GameObject * m_back;


	float m_maxProgressValue;
	Vec3f m_progressEnd;
	float m_progressP1Pos;
	float m_progressP2Pos;
	float m_progressPosRange;

	GameObject* m_progressBar;
	//Player 1
	GameObject* m_progressPoint1;
	//Player 2
	GameObject* m_progressPoint2;
};