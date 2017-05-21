#pragma once

#include "../Source/System/IElixirGame.h"

using namespace Elixir;

class ResultScene : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	void SetNextButton();

private:
};