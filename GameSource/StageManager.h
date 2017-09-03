#pragma once

#include "../Source/Includes/LESystem.h"
#include "../Source/System/IElixirGame.h"
#include "../Source/Helper/TypeHelper.h"

class StageManager
{
public:
	StageManager();
	void Initialize(Elixir::SceneManager* manager, int courseNum);

private:
	void CreateStarDust();

private:
	Elixir::SceneManager* m_manager;
};