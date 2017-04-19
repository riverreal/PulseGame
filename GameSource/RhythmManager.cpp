#include "RhythmManager.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

void RhythmManager::Initialize(Elixir::SceneManager * sceneManager)
{
	Manager = sceneManager;
	//BGM�t�@�C���ݒ�
	AudioManager::GetInstance().AddControlledMusic("filename");
	//�|�[�Y�����Đ�
	AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);
	//�Đ����Ԏ擾(�~���b)
	AudioManager::GetInstance().GetControlledMusic()->getPosition();

	//PNF�t�@�C���Q�b�g
	auto FileContents = Manager->GetFileManager()->LoadFile("Path");

}

void RhythmManager::Update(float dt)
{


}
