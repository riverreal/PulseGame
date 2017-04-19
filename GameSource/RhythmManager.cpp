#include "RhythmManager.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

void RhythmManager::Initialize(Elixir::SceneManager * sceneManager)
{
	Manager = sceneManager;
	//BGMファイル設定
	AudioManager::GetInstance().AddControlledMusic("filename");
	//ポーズ解除再生
	AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);
	//再生時間取得(ミリ秒)
	AudioManager::GetInstance().GetControlledMusic()->getPosition();

	//PNFファイルゲット
	auto FileContents = Manager->GetFileManager()->LoadFile("Path");

}

void RhythmManager::Update(float dt)
{


}
