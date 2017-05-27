#pragma once

#include "../Source/System/IElixirGame.h"
#include"PulseGame.h"

using namespace Elixir;

class MachineSelect : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();

private:
	PulseGame m_pulse;
};