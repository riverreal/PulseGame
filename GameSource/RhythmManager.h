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
	//レーン番号
	vector<int *> NotesLaneNumber;
	//タイミング
	vector<double *>NotesTiming;
	//ノーツステータス
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