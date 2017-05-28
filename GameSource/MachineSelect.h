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

private:
	PulseGame m_pulse;
	ETween<float> m_mainTEween;
	GameObject * m_title;
	GameObject * m_panel;
	GameObject * m_back;
};