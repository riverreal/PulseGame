#pragma once

#include "GameObject.h"

namespace Elixir
{
	class SceneManager;

	class Package
	{
	public:
		Package(SceneManager* manager);

		std::vector<GameObject*> LoadPackage(std::string filepath);
		void CreatePackage(std::string filepath, std::vector<GameObject*> goVec);

	private:
		std::string SerializePackage(std::vector<GameObject*> goVec);
		std::vector<GameObject*> DeserializePackage(std::string packageData);

	private:
		const char m_encKey[10] = { 'B','S','Q','Z','I','{','F','R','B','!' };
		SceneManager* m_manager;

	};
}

