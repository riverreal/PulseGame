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
	void SetNextButton();
	void TextAnimLoop(bool flag);

private:
	
	void TextImage();
	
	GameObject* m_Machine;
	GameObject* m_text;
	ETween<Vec3f> m_MachineAnim;
	ETween<Vec3f> m_textAnim;
	ETween<Vec3f> m_textAnim2;
};