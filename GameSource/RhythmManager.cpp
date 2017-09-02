#include "RhythmManager.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"
#include "../Source/Helper/ENote.h"
#include "../External Soruce/cpplinq.hpp"

using namespace Elixir;
using namespace cpplinq;

void RhythmManager::Initialize(Elixir::SceneManager * sceneManager, DIFF dif, int playerNum)
{
	Manager = sceneManager;
	m_Combo = 0;
	m_difficulty = (int)dif;
	m_PlayerNum = playerNum;
	m_spriteFrameCount = 0;
	if (m_PlayerNum != 2)
	{
		m_spriteNumber.Initialize(Manager);
		m_textParent = m_spriteNumber.GetSpriteParent();
		m_textParent->GetTransform()->Scale = Vec3f(0.15, 0.15, 0.15);
		m_textParent->GetTransform()->Position = Vec3f(550, 70, 0);
		m_spriteNumber.UpdateSprite(0);

		m_ComboLabel = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
		m_ComboLabel->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Text_Img/combo_text.png");
		m_ComboLabel->GetTransform()->Position = Vec3f(560, 25, 0);
		m_ComboLabel->GetTransform()->Scale = Vec3f(0.3, 0.3, 0.3);

		//time
		/*
		m_spriteSecondCount.Initialize(Manager);
		m_spriteSecondCount.GetSpriteParent()->GetTransform()->Scale = Vec3f(0.15, 0.15, 0.15);
		m_spriteSecondCount.GetSpriteParent()->GetTransform()->Position = Vec3f(550, 130, 0);
		m_spriteSecondCount.UpdateSprite(0);
		*/
	}

	bool isSplit = ENote::GetInstance().Notify<bool>("GetSplitScreen");
	auto screenW = GameManager::GetInstance().GetScreenWidth();
	auto halfScreenW = screenW * 0.5f;
	float centerPos = ((screenW / 4.0f) * 1) - halfScreenW;

	if (playerNum == 0 || playerNum == 1)
	{
		//prepare effect images
		m_effectTextures.push_back(Manager->GetTextureManager()->AddTexture(L"Resource/RaceEffect/warp01.png"));
		m_effectTextures.push_back(Manager->GetTextureManager()->AddTexture(L"Resource/RaceEffect/warp02.png"));
		m_effectTextures.push_back(Manager->GetTextureManager()->AddTexture(L"Resource/RaceEffect/warp03.png"));
		m_effectTextures.push_back(Manager->GetTextureManager()->AddTexture(L"Resource/RaceEffect/warp04.png"));
		m_effectTextures.push_back(Manager->GetTextureManager()->AddTexture(L"Resource/RaceEffect/warp05.png"));
		m_effectTextures.push_back(Manager->GetTextureManager()->AddTexture(L"Resource/RaceEffect/warp06.png"));
		m_speedEffect = Manager->GetCurrentScene()->CreateObject(OBJECT_2D);
		m_speedEffect->Get2DRenderer()->Texture = m_effectTextures[0];
		m_speedEffect->Get2DRenderer()->Color.a = 0.0f;
		if (isSplit)
		{
			m_speedEffect->GetTransform()->Scale.x = m_speedEffect->GetTransform()->Scale.x * (GameManager::GetInstance().GetScreenWidth() / (640*2));

			if (playerNum == 0)
			{
				m_speedEffect->GetTransform()->Position.x = -centerPos;
			}
			else
			{
				m_speedEffect->GetTransform()->Position.x = centerPos;
			}
		}
		else
		{
			m_speedEffect->GetTransform()->Scale.x = m_speedEffect->GetTransform()->Scale.x * (GameManager::GetInstance().GetScreenWidth() / 640);
		}
		
		m_speedEffect->GetTransform()->Scale.y = m_speedEffect->GetTransform()->Scale.y * (GameManager::GetInstance().GetScreenWidth() / 480);
	}

	if (isSplit)
	{
		switch (m_difficulty)
		{
		case 1:
			m_LanePosE[0].x = centerPos;
			break;
		case 2:	
			m_LanePosN[0].x = centerPos + 130;
			m_LanePosN[1].x = centerPos - 130;

			break;
		case 3:
			m_LanePosH[1].x = centerPos;
			m_LanePosH[0].x = centerPos + 130;
			m_LanePosH[2].x = centerPos - 130;
			break;
		default:
			break;
		}

		m_textParent->GetTransform()->Position.x -= halfScreenW;
		m_spriteNumber.UpdateSprite(0);
		m_ComboLabel->GetTransform()->Position.x -= halfScreenW;
	}

	if (playerNum == 1)
	{
		
		switch (m_difficulty)
		{
		case 1:
			m_LanePosE[0].x += halfScreenW;
			break;
		case 2:
			m_LanePosN[0].x += halfScreenW;
			m_LanePosN[1].x += halfScreenW;

			break;
		case 3:
			m_LanePosH[0].x += halfScreenW;
			m_LanePosH[1].x += halfScreenW;
			m_LanePosH[2].x += halfScreenW;
			break;
		default:
			break;
		}

		m_textParent->GetTransform()->Position.x += halfScreenW;
		m_spriteNumber.UpdateSprite(0);
		m_ComboLabel->GetTransform()->Position.x += halfScreenW;
	}

	m_timingBonus = 0;

	int songNum = ENote::GetInstance().Notify<int>("GetSelectedSong");

	//BGMファイル設定
	AudioManager::GetInstance().AddControlledMusic("Resource/rhythmFolder/" + CourseDataArray[songNum].music + ".mp3");
	AudioManager::GetInstance().GetControlledMusic()->setIsPaused(true);

	ENote::GetInstance().AddNote<int>("GetCombo" + playerNum, [this]() ->int {return this->GetCombo(); });
	ENote::GetInstance().AddNote<int>("GetTimingBonus" + playerNum, [this]() ->int {return this->GetTimingBonus(); });

	//PNFファイルゲット
	auto FileContents = Manager->GetFileManager()->LoadFileLines("Resource/rhythmFolder/Pnf_Folder/" + CourseDataArray[songNum].music + ".pnf");
	//lineを,で分割して代入
	for (auto & line : FileContents)
	{
		auto val = split(line, ',');
		m_NotesLaneNumber.push_back(std::stoi(val[0]));
		m_NotesTiming.push_back(std::stold(val[1]));
	}

	if (m_difficulty != 3)
	{
		for (int i = 0; i < m_NotesLaneNumber.size(); i++)
		{
			if (m_difficulty == 2)
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
			else if (m_difficulty == 1)
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
		hit2d->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/" + COLOR_PATH[0] + L"_Hit.png");
		hit2d->GetTransform()->Rotation.x = -90;
		hit2d->GetTransform()->Position.z += 5;
		hit2d->SetTag(m_PlayerNum);
		hit2d->Get2DRenderer()->Enabled = false;
		auto _status = Status(hit2d, 1000, false);
		m_NotesStatus.push_back(_status);
	}

	for (int i = 0; i < m_difficulty; i++)
	{
		//waveEfect
		auto effectWave2d = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
		effectWave2d->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Effect/" + WAVE_EFFECT_PATH[1] + L".png");
		effectWave2d->GetTransform()->Position = m_difLane[m_difficulty - 1][i];
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
		Screen2d->GetTransform()->Position = m_difLane[m_difficulty - 1][i];

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

	m_TextEffect->GetTransform()->Position = m_difLane[0][0];
	//マルチ用調整
	if (isSplit)
	{
		m_TextEffect->GetTransform()->Position = centerPos;
		if (playerNum == 1)
		{
			m_TextEffect->GetTransform()->Position.x += halfScreenW;
		}
	}
	
	m_TextEffect->GetTransform()->Position.y += 240;
	m_TextEffect->GetTransform()->Scale = m_TextEffect->GetTransform()->Scale * 0.8f;

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
			//obj->GetTransform()->Position = obj->GetTransform()->Position * GameManager::GetInstance().GetDesignScale();
			//obj->GetTransform()->Scale = obj->GetTransform()->Scale * GameManager::GetInstance().GetDesignScale();
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

					if (m_PlayerNum != 2) m_spriteNumber.UpdateSprite(m_Combo);
				}
			}
		}
	}

	for (auto& _status : m_NotesStatus)
	{
		if (_status.active)
		{
			//最終スケール + ((タイミング - 現在の再生時間) * 初期スケール) / 1000
			auto _scale = 0.9f + ((m_NotesTiming[_status.num] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition()) * m_DefaultScale) / 1000;
			_status.obj->GetTransform()->Scale = Vec3f(_scale, _scale, 1);
			_status.obj->Get2DRenderer()->Color.a = (-_scale + 1) / 3 + 1;
		}
	}

	static bool isF5Pressed = false;
	if (GetAsyncKeyState('U') & 0x8000)
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

	if (CommandInterpreter::GetInstance().isCenterNotes(m_PlayerNum) && !m_modeEasy)
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
				if (m_PlayerNum != 2)
				{
					_status.obj->Get2DRenderer()->Enabled = true;
					_status.obj->GetTransform()->Scale = Vec3f(m_DefaultScale + 1, m_DefaultScale + 1, 1);
					_status.obj->GetTransform()->Position = m_difLane[m_difficulty - 1][m_NotesLaneNumber[_status.num]];//m_LanePos[m_NotesLaneNumber[_status.num]];
					_status.obj->Get2DRenderer()->Texture = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/" + COLOR_PATH[m_NotesLaneNumber[_status.num]] + L"_Hit.png");
					_status.obj->Get2DRenderer()->Color.a = 0;
				}
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

	/*if (m_PlayerNum != 2)
	{
		m_flameCount += 2;
		if (m_flameCount > 60)
		{
			Log() << "sec:"<<m_secCount;
			Log() << "music:" << AudioManager::GetInstance().GetControlledMusic()->getPlayPosition();
			m_flameCount = 0;
			m_secCount++;
			m_spriteSecondCount.UpdateSprite(m_secCount);
			if (m_secCount > 60)
			{
				m_secCount = 0;
			}
		}
	}*/

	if (m_PlayerNum == 0 || m_PlayerNum == 1)
	{
		m_speedEffect->Get2DRenderer()->Color.a = MathHelper::Min(((float)m_Combo) / 150.0f, 0.75f);

		m_spriteFrameCount++;
		if (m_spriteFrameCount >= 15.0f)
		{
			m_spriteFrameCount = 0;
		}

		m_speedEffect->Get2DRenderer()->Texture = m_effectTextures[(int)floor(m_spriteFrameCount / 2.5f)];
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

	if (m_PlayerNum != 2) m_spriteNumber.UpdateSprite(m_Combo);

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
