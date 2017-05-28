#pragma once

#include "../Source/System/IElixirGame.h"
#include"SongSelect.h"

using namespace Elixir;

class ModeScene : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	void ModeSelect_Left();
	void ModeSelect_Right();
	bool GetSplitScreen();

private:
	bool m_splitScreen;
	SongSelect m_song;
	GameObject* left;
	GameObject* right;
};