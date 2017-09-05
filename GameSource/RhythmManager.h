#pragma once
#include "../Source/System/IElixirGame.h"
#include <vector>
#include "../Source/Helper/TypeHelper.h"
#include "../Source/Helper/ETween.h"
#include "CourseSelect.h"
#include"CommandInterpreter.h"
#include "SpriteNumber.h"


enum DIFF
{
	EASY = 1,
	NORMAL = 2,
	HARD = 3
};

class RhythmManager
{
public:
	void Initialize(Elixir::SceneManager* sceneManager, DIFF dif, int playerNum);

	void Update(float dt);

	int GetCombo();
	int GetTimingBonus();
	std::vector<double> GetNotesTiming();
	std::vector<int>GetNotesLaneNumber();

	//���莞��
	const int GREAT_TIME = 120;
	const int GOOD_TIME = 220;
	const int BAD_TIME = 340;

private:
	//�m�[�c�X�e�[�^�X
	struct Status {
			Elixir::GameObject* obj;
			int num;
			bool active;
			Status(Elixir::GameObject* _obj, int _num, bool _active)
			{
				obj = _obj;
				num = _num;
				active = _active;
			}
			Status()
				:obj(nullptr),
				num(0),
				active(false)
			{}
	};

	//���[���ԍ�
	std::vector<int> m_NotesLaneNumber;
	//�^�C�~���O
	std::vector<double>m_NotesTiming;
	//�m�[�c�X�e�[�^�X���X�g
	std::vector<Status>m_NotesStatus;
	//�����|�W�V����(���[��)
	//HARD
	Elixir::Vec3f m_LanePosH[3]
	{
		Elixir::Vec3f(200, -410, 0),
		Elixir::Vec3f(0, -440, 0),
		Elixir::Vec3f(-200, -410, 0)
	};
	//NORMAL
	Elixir::Vec3f m_LanePosN[2]
	{
		Elixir::Vec3f(200, -410, 0),
		Elixir::Vec3f(-200, -410, 0)
	};
	//EASY
	Elixir::Vec3f m_LanePosE[1]
	{
		Elixir::Vec3f(0, -440, 0),
	};
	//��Փx�ʔz��
	Elixir::Vec3f *m_difLane[3]
	{
		m_LanePosE,
		m_LanePosN,
		m_LanePosH
	};

	//�����X�P�[��
	const float m_DefaultScale = 3.0f;
	
	//�摜��
	const std::wstring COLOR_PATH[3]
	{
		L"Red",
		L"Bule",
		L"Yellow"
	};
	const std::wstring TEXT_EFFECT_PATH[4]
	{
		L"Great",
		L"Good",
		L"Bad",
		L"Miss"
	};
	const std::wstring WAVE_EFFECT_PATH[2]
	{
		L"innerRingEffect",
		L"ringEffect"
	};
	
	//�^�C�~���O�J�E���g
	int m_TimingCount = 0;
	//�R���{
	int m_Combo = 0;
	int m_timingBonus;
	bool m_Press = false;
	
	bool m_modeEasy = false;
	bool m_modeNormal = false;

	int m_PlayerNum;

	//�����G�t�F�N�g
	Elixir::GameObject *m_TextEffect;
	//�R���{���x��
	Elixir::GameObject *m_ComboLabel;
	
	Status* MinStatus(int n);
	//����
	void HitTimingCheck(Status* _status);

	//animation
	Elixir::ETween<F32> m_tween;
	Elixir::ETween<F32> m_textAnim;
	Elixir::ETween<F32> m_inputAnim;
	Elixir::ETween<F32> m_waveAnim[3];

	std::vector<U32> m_effectTextures;
	float m_effectAlpha;
	Elixir::GameObject* m_speedEffect;
	float m_spriteFrameCount;
	SpriteNumber m_spriteNumber;

	//For convinience
	Elixir::SceneManager* Manager;
	Elixir::GameObject* m_textParent;

	//time
	float m_flameCount = 0;
	int m_secCount = 0;
	int m_minCount = 0;
	SpriteNumber m_spriteSecondCount;
	SpriteNumber m_spriteMinuteCount;

	int m_difficulty;
};