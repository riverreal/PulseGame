#pragma once

#include "../Source/System/IElixirGame.h"
#include"MachineSelect.h"

using namespace Elixir;

class ModeScene : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	void ModeSelect_Left();
	void ModeSelect_Right();

private:
	SongSelect m_song;
	GameObject* left;
	GameObject* right;
};