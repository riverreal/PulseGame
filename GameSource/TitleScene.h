#pragma once

#include "../Source/System/IElixirGame.h"

using namespace Elixir;

class title : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();


};