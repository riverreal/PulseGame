#pragma once
#include "../Source/System/IElixirGame.h"
#include <vector>
#include "../Source/Helper/TypeHelper.h"
#include "../Source/Helper/ETween.h"

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
		Elixir::Vec3f( 300,-300,0 ),
		Elixir::Vec3f( 0,-330,0 ),
		Elixir::Vec3f( -300,-300,0 )
	};

	//初期スケール
	const float m_DefaultScale = 3.0f;
	//ファイルネーム
	const std::string FILE_NAME = "LarsM-Lovers";
	//画像名
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

	//判定時間
	const int GREAT_TIME = 100;
	const int GOOD_TIME = 230;
	const int BAD_TIME = 340;
	
	//タイミングカウント
	int m_TimingCount = 0;
	//コンボ
	int m_Combo = 0;
	
	bool m_Press = false;

	//文字エフェクト
	Elixir::GameObject *m_TextEffect;
	
	Status* MinStatus(int n);
	//判定
	void HitTimingCheck(Status* _status);

	//animation
	Elixir::ETween<F32> m_tween;
	Elixir::ETween<F32> m_textAnim;
	Elixir::ETween<F32> m_inputAnim;
	Elixir::ETween<F32> m_waveAnim[3];
	
	//For convinience
	Elixir::SceneManager* Manager;
};