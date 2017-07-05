#include "Package.h"
#include "../Helper/GeneralHelper.h"

using namespace Elixir;

Package::Package(SceneManager * manager)
{
	m_manager = manager;
}

void Package::LoadPackage(std::string filepath)
{
	std::string packageData = m_manager->GetFileManager()->LoadFile(filepath);

}

void Package::CreatePackage(std::string filepath, std::vector<GameObject*> goVec)
{
}

std::string Package::SerializePackage(std::vector<GameObject*> goVec)
{


	return std::string();
}

std::vector<GameObject*> Package::DeserializePackage(std::string packageData)
{
	return std::vector<GameObject*>();
}
