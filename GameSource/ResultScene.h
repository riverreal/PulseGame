#pragma once

#include "../Source/System/IElixirGame.h"
#include "../Source/Helper/ETween.h"

using namespace Elixir;

class ResultScene : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	void BlackImage();
	void StartAnim();
	void BackAnim();
	void ChangeScene();
	void BackScene();

private:
	ETween<float> m_mainTEween;
	GameObject * m_title;
	GameObject * m_back;
};