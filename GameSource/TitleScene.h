#pragma once

#include "../Source/System/IElixirGame.h"
#include "ModeScene.h"
#include "../Source/Helper/ETween.h"
#include "../External Soruce/rxcpp/rx.hpp"

using namespace Elixir;

class TitleScene : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	void SetImage();
	void ChangeScene();
	void BlackImage();
	void BackAnim();
	void StartAnim();
	void EnableInput();
	void CrystalAnimation(bool up);
private:
	

private:
	bool m_isNextButtonPressed;
	bool m_inputEnabled;

	ModeScene m_mode;
	ETween<float> m_mainTEween;
	ETween<float> m_crystalTween;
	ETween<float> m_crystalTween2;
	GameObject* m_crystal;
	GameObject * m_back;
	rxcpp::subjects::subject<bool> m_trigger;
};