#include "RhythmManager.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

void RhythmManager::Initialize(Elixir::SceneManager * sceneManager)
{
	Manager = sceneManager;
	//BGM�t�@�C���ݒ�
	//AudioManager::GetInstance().AddControlledMusic("filename");
	//�|�[�Y�����Đ�(�f�t�H���g�Ń|�[�Y�ݒ�)
	//AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);
	//�Đ����Ԏ擾(�~���b)
	//AudioManager::GetInstance().GetControlledMusic()->getPosition();

	//PNF�t�@�C���Q�b�g
	auto FileContents = Manager->GetFileManager()->LoadFileLines("C:/Users/yuu2/Documents/PnfFileCreateTool/BPMandOffset/BPMandOffset/bin/Pnf_Folder/Dash.pnf");
	ElixirLog(FileContents[0]);
	for (auto & line: FileContents)
	{
		
	}

}

void RhythmManager::Update(float dt)
{


}
