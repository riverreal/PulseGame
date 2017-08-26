#pragma once

#include "../Source/System/IElixirGame.h"
#include "MachineSelect.h"
#include "../Source/Helper/ETween.h"

using namespace Elixir;

class SongSelect : public IElixirGame
{
public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	
	int GetSelectedSong();
	void EnableInput();

private:
	void SetImage();
	void BlackImage();
	void StartAnim();
	void BackAnim();
	void ChangeScene();
	void BackScene();

	void SetPlanets();

private:
	bool m_inputEnabled;
	MachineSelect m_machine = {};
	ETween<float> m_mainTEween;
	GameObject * m_title;
	GameObject * m_panel;
	GameObject * m_start;
	GameObject * m_back;
	GameObject* m_planets;
	GameObject* m_courseInfo;
	Vec3f m_camPos;

	const std::wstring COURSE_INFO_PATH[3]
	{
		L"Resource/CourseScene/Course-01.png",
		L"Resource/CourseScene/Course-02.png",
		L"Resource/CourseScene/Course-03.png"
	};

	int m_selectedSong;
	int m_maxSelSongs;
};