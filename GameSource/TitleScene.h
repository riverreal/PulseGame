#pragma once

#include "../Source/System/IElixirGame.h"
#include "ModeScene.h"
#include "PulseGame.h"

using namespace Elixir;

class TitleSecne : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();

private:
	ModeScene m_mode;
};