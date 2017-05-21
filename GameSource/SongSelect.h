#pragma once

#include "../Source/System/IElixirGame.h"
#include"MachineSelect.h"

using namespace Elixir;

class SongSelect : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();

private:
	MachineSelect m_machine;
};