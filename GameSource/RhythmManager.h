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
	//���[���ԍ�
	vector<int *> NotesLaneNumber;
	//�^�C�~���O
	vector<double *>NotesTiming;
	//�m�[�c�X�e�[�^�X
	struct NoteStatus {
			GameObject obj;
			int num;
			int direction;
			bool active;
			NoteStatus(GameObject _obj, int _num, int _dir, bool _active)
			{
				obj = _obj;
				num = _num;
				direction = _dir;
				active = _active;
			}
	};


	//For convinience
	Elixir::SceneManager* Manager;
};