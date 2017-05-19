#pragma once

#include "../Source/System/IElixirGame.h"

using namespace Elixir;

class result : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	void SetNextButton();


};