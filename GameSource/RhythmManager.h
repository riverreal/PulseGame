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
	//ノーツステータス
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

	//レーン番号
	std::vector<int> m_NotesLaneNumber;
	//タイミング
	std::vector<double>m_NotesTiming;
	//ノーツステータスリスト
	std::vector<Status>m_NotesStatus;
	
	//初期ポジション(レーン)
	Elixir::Vec3f m_LanePos[3]
	{
		Elixir::Vec3f( 0,0,0 ),
		Elixir::Vec3f( 0,0,0 ),
		Elixir::Vec3f( 0,0,0 )
	};
	

	//初期スケール
	const float m_DefaultScale = 5.0f;
	//タイミングカウント
	int m_TimingCount = 0;
	//コンボ
	int m_Combo = 0;
	//ファイルネーム
	const std::string FILE_NAME = "Dash";
	//判定時間
	const int GREAT_TIME = 150;
	const int GOOD_TIME = 350;
	const int BAD_TIME = 783;
	//判定
	void HitTimingCheck(Status);

	Status MinStatus(int n);
	
	bool m_Press = false;

	//For convinience
	Elixir::SceneManager* Manager;
};