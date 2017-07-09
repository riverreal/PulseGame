#pragma once
#include <vector>
namespace Elixir {

	class GameObject;
	struct  Vec3f;
	class SceneManager;
}

class SpriteNumber
{
public:
	
	void Initialize(Elixir::SceneManager* sceneManager);
	void UpdateSprite(int value);
	Elixir::GameObject* GetSpriteParent();

private:

	Elixir::GameObject* m_parentsObj;

	std::wstring m_spritePath[10]
	{
		L"Resources/NumberSprite/0.png",
		L"Resources/NumberSprite/1.png",
		L"Resources/NumberSprite/2.png",
		L"Resources/NumberSprite/3.png",
		L"Resources/NumberSprite/4.png",
		L"Resources/NumberSprite/5.png",
		L"Resources/NumberSprite/6.png",
		L"Resources/NumberSprite/7.png",
		L"Resources/NumberSprite/8.png",
		L"Resources/NumberSprite/9.png",
	};
	
	//For convinience
	Elixir::SceneManager* Manager;
};

