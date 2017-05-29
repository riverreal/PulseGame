#include "RhythmManager.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/ENote.h"
#include "../External Soruce/cpplinq.hpp"

using namespace Elixir;
using namespace cpplinq;

void RhythmManager::Initialize(Elixir::SceneManager * sceneManager , DIFF dif,int playerNum)
{
	Manager = sceneManager;
	m_Combo = 0;
	int NumofLane = (int)dif;
	m_PlayerNum = playerNum;

	bool isSplit = ENote::GetInstance().Notify<bool>("GetSplitScreen");

	m_LanePos[0] = Vec3f(200, -210, 0);
	m_LanePos[1] = Vec3f(0, -240, 0);
	m_LanePos[2] = Vec3f(-200, -210, 0);

	if (isSplit)
	{
		m_LanePos[1].x = ((1280.0f / 4) * 1) - 640.0f;
		m_LanePos[0].x = m_LanePos[1].x + 130;
		m_LanePos[2].x = m_LanePos[1].x - 130;
	}

	if (playerNum == 1)
	{
		m_LanePos[0].x += 640.0f;
		m_LanePos[1].x += 640.0f;
		m_LanePos[2].x += 640.0f;
	}

	m_timingBonus = 0;
	//BGMファイル設定
	AudioManager::GetInstance().AddControlledMusic("Resource/rhythmFolder/" + CourseDataArray[CourseID::hitorigoto].music +".mp3");
	AudioManager::GetInstance().GetControlledMusic()->setIsPaused(true);

	ENote::GetInstance().AddNote<int>("GetCombo" + playerNum, [this]() ->int {return this->GetCombo(); });
	ENote::GetInstance().AddNote<int>("GetTimingBonus" + playerNum, [this]() ->int {return this->GetTimingBonus(); });

	//PNFファイルゲット
	auto FileContents = Manager->GetFileManager()->LoadFileLines("Resource/rhythmFolder/Pnf_Folder/" + CourseDataArray[CourseID::hitorigoto].music + ".pnf");
	//lineを,で分割して代入
	for (auto & line : FileContents)
	{
		auto val = split(line, ',');
		m_NotesLaneNumber.push_back(std::stoi(val[0]));
		m_NotesTiming.push_back(std::stold(val[1]));
	}
	//LaneofNum
	if (NumofLane != 3)
	{	
		for (int i = 0; i < m_NotesLaneNumber.size(); i++)
		{
			if (NumofLane == 2)
			{
				if (m_NotesLaneNumber[i] == 2)
				{
					if (i != 0)
						m_NotesLaneNumber[i] = m_NotesLaneNumber[i - 1];
					else
						m_NotesLaneNumber[i] = rand() % 2;
				}
				m_modeNormal = true;
			}
			else if (NumofLane == 1)
			{
				m_NotesLaneNumber[i] = 0;
				m_modeEasy = true;
			}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		//Notes
		auto hit2d = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
		hit2d->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/" + COLOR_PATH[0]+L"_Hit.png");
		hit2d->GetTransform()->Rotation.x = -90;
		hit2d->GetTransform()->Position.z += 5;
		hit2d->SetTag(m_PlayerNum);
		hit2d->Get2DRenderer()->Enabled = false;
		auto _status = Status(hit2d, 1000, false);
		m_NotesStatus.push_back(_status);		
	}

	for (int i = 0; i < NumofLane; i++)
	{	
		//waveEfect
		auto effectWave2d = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
		effectWave2d->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Effect/" + WAVE_EFFECT_PATH[1] + L".png");
		effectWave2d->GetTransform()->Position = m_LanePos[i];
		effectWave2d->Get2DRenderer()->Color.a = 0.7f;
		effectWave2d->SetTag(m_PlayerNum);

		//waveEffectAnim
		m_waveAnim[i] = m_waveAnim[i].From(&effectWave2d->GetTransform()->Scale.x).To(2.5f).Time(0.3f)
			.From(&effectWave2d->Get2DRenderer()->Color.a).To(0).Time(0.3f)
			.From(&effectWave2d->GetTransform()->Scale.y).To(2.5f).Time(0.3f);

		effectWave2d->Get2DRenderer()->Color.a = 0.0f;

		//hitBox
		auto Screen2d = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
		Screen2d->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/" + COLOR_PATH[i] + L"_Box.png");
		Screen2d->GetTransform()->Position = m_LanePos[i];
		Screen2d->SetTag(m_PlayerNum);
		
		//inputAnimation
		ETween<F32> endAnim;
		endAnim = endAnim.From(&Screen2d->GetTransform()->Scale.x).To(1.0f).Time(0.1f)
			.From(&Screen2d->GetTransform()->Scale.y).To(1.0f).Time(0.1f);
		
		m_inputAnim = m_inputAnim.From(&Screen2d->GetTransform()->Scale.x).To(0.9f).Time(0.1f)
			.From(&Screen2d->GetTransform()->Scale.y).To(0.9f).Time(0.1f)
			.OnFinishChain(&endAnim);
	}

	m_TextEffect = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
	m_TextEffect->SetTag(m_PlayerNum);
	m_TextEffect->GetTransform()->Position = Vec3f(0, -75, 0);
	
	m_textAnim = m_textAnim.From(&m_TextEffect->Get2DRenderer()->Color.a).To(0.0f).Time(1.0f)
		.From(&m_TextEffect->GetTransform()->Position.y).To(m_TextEffect->GetTransform()->Position.y + 30.0f).Time(1.0f);

	m_TextEffect->Get2DRenderer()->Color.a = 0.0f;

	if (playerNum == 2)
	{
		from(Manager->GetCurrentScene()->GetChildren())
			>> where([playerNum](GameObject* obj) {return obj->Get2DRenderer() != nullptr && obj->GetTag() == playerNum; })
			>> for_each([](GameObject* obj) {
			obj->GetTransform()->Dynamic = false;
			obj->Get2DRenderer()->Enabled = false;
			obj->Get2DRenderer()->Color.a = 0.0f;
		});
	}
	else
	{
		from(Manager->GetCurrentScene()->GetChildren())
			>> where([](GameObject* obj) {return obj->Get2DRenderer() != nullptr; })
			>> for_each([](GameObject* obj) {
			obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale();
			obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();

		});
	}
}

void RhythmManager::Update(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		auto lastNote = MinStatus(i);
		if (lastNote != nullptr)
		{
			if (lastNote->obj != nullptr && lastNote->active && lastNote->obj->Get2DRenderer()->Enabled)
			{
				if (m_NotesTiming[lastNote->num] + BAD_TIME*0.5f + 20 < (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition())
				{
					lastNote->active = false;
					m_Combo = 0;
					m_timingBonus = 0;
					lastNote->obj->Get2DRenderer()->Enabled = false;
					m_TextEffect->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Text_Img/" + TEXT_EFFECT_PATH[3] + L".png");
					//m_TextEffect->Get2DRenderer()->Enabled = true;
					m_tween.AddTweens(m_textAnim.GetTweens());
				}
			}
		}
	}

	for (auto& _status : m_NotesStatus)
	{
		if (_status.active)
		{
			//最終スケール + ((タイミング - 現在の再生時間) * 初期スケール) / 1000
			auto _scale = 0.9f + ((m_NotesTiming[_status.num] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition()) * m_DefaultScale ) /1000;
			_status.obj->GetTransform()->Scale = Vec3f(_scale, _scale, 1);
			_status.obj->Get2DRenderer()->Color.a = (-_scale + 1) / 3 + 1;
		}		
	}

	static bool isF5Pressed = false;
	if (GetAsyncKeyState(VK_F5) & 0x8000)
	{
		if (!isF5Pressed)
		{
			isF5Pressed = true;

			for (auto &status : m_NotesStatus)
			{
				status.active = false;
				status.obj->Get2DRenderer()->Enabled = false;
				status.num = 1000;
			}
			m_TimingCount = 0;
			AudioManager::GetInstance().GetControlledMusic()->setPlayPosition(0);
			AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);
		}
	}
	else
	{
		isF5Pressed = false;
	}

	#pragma region input key 

	if (CommandInterpreter::GetInstance().isCenterNotes(m_PlayerNum) && !m_modeEasy )
	{
		if (!m_Press)
		{
			AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);
			m_Press = true;
			HitTimingCheck(MinStatus(1));
			m_tween.AddTween(m_inputAnim.GetTweens()[2]);
			m_tween.AddTween(m_inputAnim.GetTweens()[3]);
		}
	}
	else if (CommandInterpreter::GetInstance().isRightNotes(m_PlayerNum))
	{
		if (!m_Press)
		{
			AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);
			m_Press = true;
			HitTimingCheck(MinStatus(0));
			m_tween.AddTween(m_inputAnim.GetTweens()[0]);
			m_tween.AddTween(m_inputAnim.GetTweens()[1]);
		}
	}
	else if (CommandInterpreter::GetInstance().isLeftNotes(m_PlayerNum) && !m_modeEasy && !m_modeNormal)
	{
		if (!m_Press)
		{
			AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);
			m_Press = true;
			HitTimingCheck(MinStatus(2));
			m_tween.AddTween(m_inputAnim.GetTweens()[4]);
			m_tween.AddTween(m_inputAnim.GetTweens()[5]);
		}
	}

	else
		m_Press = false;
	

#pragma endregion

	if (m_TimingCount >= m_NotesTiming.size())return;
	
	if (m_NotesTiming[m_TimingCount] - 1000 < (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition())
	{
		for (auto& _status : m_NotesStatus)
		{
			if (!_status.active)
			{
				_status.active = true;
				_status.num = m_TimingCount;
				_status.obj->Get2DRenderer()->Enabled = true;
				_status.obj->GetTransform()->Scale = Vec3f(m_DefaultScale + 1, m_DefaultScale + 1, 1);
				_status.obj->GetTransform()->Position = m_LanePos[m_NotesLaneNumber[_status.num]];
				_status.obj->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/" + COLOR_PATH[m_NotesLaneNumber[_status.num]] + L"_Hit.png");
				_status.obj->Get2DRenderer()->Color.a = 0;
				break;
			}
		}
		m_TimingCount++;
	}

	//Make it replayable
	//static bool reloadedSong = false;
	if (AudioManager::GetInstance().GetControlledMusic()->isFinished())
	{
		AudioManager::GetInstance().AddControlledMusic("Resource/rhythmFolder/" + CourseDataArray[CourseID::hitorigoto].music + ".mp3");
		AudioManager::GetInstance().GetControlledMusic()->setIsPaused(true);
		AudioManager::GetInstance().GetControlledMusic()->setPlayPosition(0);

		//ElixirLog("Ended!");
	}
	m_tween.Update(dt);
}

int RhythmManager::GetCombo()
{
	return m_Combo;
}

int RhythmManager::GetTimingBonus()
{
	return m_timingBonus;
}

std::vector<double> RhythmManager::GetNotesTiming()
{
	return m_NotesTiming;
}

std::vector<int> RhythmManager::GetNotesLaneNumber()
{
	return m_NotesLaneNumber;
}

void RhythmManager::HitTimingCheck(Status* _status)
{
	if (_status == nullptr)
		return;

	if (_status->obj == nullptr)
		return;

	int timing = abs(m_NotesTiming[_status->num] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition());
	bool missFlag = false;

	if (timing <= GREAT_TIME / 2)
	{
		_status->active = false;
		m_Combo++;
		m_timingBonus += 2;
		_status->obj->Get2DRenderer()->Enabled = false;
		m_TextEffect->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Text_Img/" + TEXT_EFFECT_PATH[0] + L".png");
		//m_TextEffect->Get2DRenderer()->Enabled = true;
		m_tween.AddTweens(m_textAnim.GetTweens());

	}
	else if (timing <= GOOD_TIME / 2)
	{
		_status->active = false;
		m_Combo++;
		m_timingBonus++;
		_status->obj->Get2DRenderer()->Enabled = false;
		m_TextEffect->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Text_Img/" + TEXT_EFFECT_PATH[1] + L".png");
		//m_TextEffect->Get2DRenderer()->Enabled = true;
		m_tween.AddTweens(m_textAnim.GetTweens());
	}
	else if (timing <= BAD_TIME / 2)
	{
		_status->active = false;
		m_Combo++;
		
		_status->obj->Get2DRenderer()->Enabled = false;
		m_TextEffect->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Text_Img/" + TEXT_EFFECT_PATH[2] + L".png");
		//m_TextEffect->Get2DRenderer()->Enabled = true;
		m_tween.AddTweens(m_textAnim.GetTweens());
	}
	else
	{
		_status->active = false;
		m_Combo = 0;
		m_timingBonus = 0;
		_status->obj->Get2DRenderer()->Enabled = false;
		m_TextEffect->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Text_Img/" + TEXT_EFFECT_PATH[3] + L".png");
		//m_TextEffect->Get2DRenderer()->Enabled = true;
		m_tween.AddTweens(m_textAnim.GetTweens());
		missFlag = true;
	}

	if (!missFlag)
	{
		m_tween.AddTweens(m_waveAnim[m_NotesLaneNumber[_status->num]].GetTweens());
	}
}
//各レーンの最小値を持つ構造体取得関数
RhythmManager::Status* RhythmManager::MinStatus(int n)
{
	Status* val = nullptr;
	for (auto &status : m_NotesStatus)
	{
		if (status.active)
		{
			if (m_NotesLaneNumber[status.num] == n)
			{
				if (val == nullptr)
				{
					val = &status;
					continue;
				}
				if (status.num <= val->num)
					val = &status;
			}
		}
	}
	return val;
};
