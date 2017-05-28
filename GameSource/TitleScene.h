#pragma once

#include "../Source/System/IElixirGame.h"
#include "ModeScene.h"
#include "../Source/Helper/ETween.h"

using namespace Elixir;

class TitleScene : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	void ChangeScene();
	void BlackImage();
	void BackAnim();
	void StartAnim();

private:
	ModeScene m_mode;
	ETween<float> m_mainTEween;
	GameObject * m_back;
};