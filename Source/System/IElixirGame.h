#pragma once

#include<string>

namespace Elixir
{
	class SceneManager;
	class Scene;
	class GameObject;
	class Vec3f;
	class Vec4f;
}

class IElixirGame
{
public:
	virtual void Init() = 0;
	virtual void Update(float dt) = 0;

	void StartScene(std::string SceneName);

protected:
	Elixir::SceneManager* Manager;
	Elixir::Scene* ThisScene;
};
