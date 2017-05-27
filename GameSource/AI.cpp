#include"AI.h"
#include"../Source/System/Audio/AudioManager.h"
#include"../Source/Helper/ENote.h"
using namespace Elixir;
AI::~AI()
{
}

void AI::Initialize(Elixir::SceneManager* sceneManager, int playerNum, PlayerShip* pship, RhythmManager* rhythm)
{
	m_playerNum = playerNum;

	m_NotesTiming = rhythm->GetNotesTiming();
	m_NotesLaneNumber = rhythm->GetNotesLaneNumber();
	
	m_rhythm = rhythm;
	m_ship = pship;

	m_randNum = rand() % 100;
	
	m_diff = ENote::GetInstance().Notify<int>("AiDiff");
	//m_diff = 1;

}

void AI::Update(float dt)
{
	
	CommandInterpreter::GetInstance().setRightNotes(m_playerNum, false);
	CommandInterpreter::GetInstance().setCenterNotes(m_playerNum, false);
	CommandInterpreter::GetInstance().setLeftNotes(m_playerNum, false);

	if(!m_musicFinish)
		RhythmHitCheck();
}

void AI::RhythmHitCheck()
{
	if (m_randNum > m_HitDiff[m_diff][0])
	{
		if (m_NotesTiming[m_TimingCount] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition() <= m_rhythm->GREAT_TIME / 2)
		{
			if (m_NotesLaneNumber[m_TimingCount] == 0)
				CommandInterpreter::GetInstance().setRightNotes(m_playerNum, true);
			else if (m_NotesLaneNumber[m_TimingCount] == 1)
				CommandInterpreter::GetInstance().setCenterNotes(m_playerNum, true);
			else if (m_NotesLaneNumber[m_TimingCount] == 2)
				CommandInterpreter::GetInstance().setLeftNotes(m_playerNum, true);

			if (m_TimingCount != m_NotesLaneNumber.size() - 1)
				m_TimingCount++;
			else
				m_musicFinish = true;
			
			m_randNum = rand() % 100;
		}
	}
	else if (m_randNum > m_HitDiff[m_diff][1])
	{
		if (m_NotesTiming[m_TimingCount] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition() <= m_rhythm->GOOD_TIME / 2)
		{
			if (m_NotesLaneNumber[m_TimingCount] == 0)
				CommandInterpreter::GetInstance().setRightNotes(m_playerNum, true);
			else if (m_NotesLaneNumber[m_TimingCount] == 1)
				CommandInterpreter::GetInstance().setCenterNotes(m_playerNum, true);
			else if (m_NotesLaneNumber[m_TimingCount] == 2)
				CommandInterpreter::GetInstance().setLeftNotes(m_playerNum, true);

			if (m_TimingCount != m_NotesLaneNumber.size() - 1)
				m_TimingCount++;
			else
				m_musicFinish = true;
			
			m_randNum = rand() % 100;
		}
	}
	else if (m_randNum > m_HitDiff[m_diff][2])
	{
		if (m_NotesTiming[m_TimingCount] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition() <= m_rhythm->BAD_TIME / 2)
		{
			if (m_NotesLaneNumber[m_TimingCount] == 0)
				CommandInterpreter::GetInstance().setRightNotes(m_playerNum, true);
			else if (m_NotesLaneNumber[m_TimingCount] == 1)
				CommandInterpreter::GetInstance().setCenterNotes(m_playerNum, true);
			else if (m_NotesLaneNumber[m_TimingCount] == 2)
				CommandInterpreter::GetInstance().setLeftNotes(m_playerNum, true);

			if (m_TimingCount != m_NotesLaneNumber.size() - 1)
				m_TimingCount++;
			else
				m_musicFinish = true;
			
			m_randNum = rand() % 100;
		}
	}
	else
	{
		if (m_NotesTiming[m_TimingCount] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition() <= m_rhythm->BAD_TIME*0.5f + 20)
		{
			if (m_NotesLaneNumber[m_TimingCount] == 0)
				CommandInterpreter::GetInstance().setRightNotes(m_playerNum, true);
			else if (m_NotesLaneNumber[m_TimingCount] == 1)
				CommandInterpreter::GetInstance().setCenterNotes(m_playerNum, true);
			else if (m_NotesLaneNumber[m_TimingCount] == 2)
				CommandInterpreter::GetInstance().setLeftNotes(m_playerNum, true);

			if (m_TimingCount != m_NotesLaneNumber.size() - 1)
				m_TimingCount++;
			else
				m_musicFinish = true;
			
			m_randNum = rand() % 100;
		}
	}
}
