#pragma once
#include "../Source/System/IElixirGame.h"
#include <vector>
#include "../Source/Helper/TypeHelper.h"

class RhythmManager
{
public:
	void Initialize(Elixir::SceneManager* sceneManager);

	void Update(float dt);

private:
	
	//For convinience
	Elixir::SceneManager* Manager;
};