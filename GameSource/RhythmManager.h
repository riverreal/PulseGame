#pragma once
#include "../Source/System/IElixirGame.h"
#include <vector>
#include "../Source/Helper/TypeHelper.h"
#include "../Source/Helper/ETween.h"

enum Difficulty
{
	Easy = 1,
	Normal = 2,
	Hard = 3
};

class RhythmManager
{
public:
	void Initialize(Elixir::SceneManager* sceneManager , Difficulty dif);

	void Update(float dt);

	int GetCombo();
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
	Elixir::Vec3f m_LanePos[3]
	{
		Elixir::Vec3f( 200,-150,0 ),
		Elixir::Vec3f( 0,-180,0 ),
		Elixir::Vec3f( -200,-150,0 )
	};

	//�����X�P�[��
	const float m_DefaultScale = 3.0f;
	//�t�@�C���l�[��
	const std::string FILE_NAME = "LarsM-Lovers";
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

	//���莞��
	const int GREAT_TIME = 100;
	const int GOOD_TIME = 230;
	const int BAD_TIME = 340;
	
	//�^�C�~���O�J�E���g
	int m_TimingCount = 0;
	//�R���{
	int m_Combo = 0;
	
	bool m_Press = false;
	
	bool m_modeEasy = false;
	bool m_modeNormal = false;

	//�����G�t�F�N�g
	Elixir::GameObject *m_TextEffect;
	
	Status* MinStatus(int n);
	//����
	void HitTimingCheck(Status* _status);

	//animation
	Elixir::ETween<F32> m_tween;
	Elixir::ETween<F32> m_textAnim;
	Elixir::ETween<F32> m_inputAnim;
	Elixir::ETween<F32> m_waveAnim[3];


	
	//For convinience
	Elixir::SceneManager* Manager;
};