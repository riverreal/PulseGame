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
		Elixir::Vec3f( 2,0,10 ),
		Elixir::Vec3f( 0,2,10 ),
		Elixir::Vec3f( -2,0,10 )
	};
	

	//�����X�P�[��
	const float m_DefaultScale = 3.0f;
	//�^�C�~���O�J�E���g
	int m_TimingCount = 0;
	//�R���{
	int m_Combo = 0;
	//�t�@�C���l�[��
	const std::string FILE_NAME = "LarsM-Lovers";
	const std::wstring IMAGE_NAME[3]
	{
		L"Red",
		L"Bule",
		L"Yellow"
	};
	//���莞��
	const int GREAT_TIME = 60;
	const int GOOD_TIME = 150;
	const int BAD_TIME = 300;
	//����
	void HitTimingCheck(Status* _status);

	Status* MinStatus(int n);
	
	bool m_Press = false;

	//For convinience
	Elixir::SceneManager* Manager;
};