#include"AI.h"


void AI::Initialize(Elixir::SceneManager* sceneManager, int playerNum, PlayerShip* pship, RhythmManager* rhythm)
{
	m_playerNum = playerNum;

	m_NotesTiming = rhythm->GetNotesTiming();
	m_NotesLaneNumber = rhythm->GetNotesLaneNumber();
	
	
}

void AI::Update(float dt)
{
	


}

