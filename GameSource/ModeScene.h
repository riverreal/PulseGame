#pragma once

#include "../Source/System/IElixirGame.h"
#include "SongSelect.h"
#include "../Source/Helper/ETween.h"

using namespace Elixir;

class ModeScene : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	bool GetSplitScreen();
	void EnableInput();

private:
	void StartAnim();
	void BackAnim();
	void BlackImage();
	void ChangeScene();


private:
	bool m_inputEnabled;
	bool m_splitScreen;
	ETween<float> m_mainTEween;
	ETween<Vec3f> m_targetTween;
	SongSelect m_song;
	GameObject * m_back;
	GameObject * m_modetitle;
	Vec3f m_camPos;
	Vec3f m_camLookAt;
	Vec3f m_singleTarget;
	Vec3f m_multiTarget;
	bool m_singlePlayerMode;
	int m_waitFrame;
};