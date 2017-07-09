#include "Package.h"
#include "../Helper/GeneralHelper.h"
#include "../../External Soruce/jsoncpp/json/json.h"
#include "SceneManager.h"

using namespace Elixir;


Package::Package(SceneManager * manager)
{
	m_manager = manager;
}

std::vector<GameObject*> Package::LoadPackage(std::string filepath)
{
	std::string packageData = m_manager->GetFileManager()->LoadFile(filepath);
	//auto decrypted = XOREncryptDecrypt(packageData, m_encKey);
	return DeserializePackage(packageData);
}

void Package::CreatePackage(std::string filepath, std::vector<GameObject*> goVec)
{
	auto content = SerializePackage(goVec);
	//auto encrypted = XOREncryptDecrypt(content, m_encKey);
	m_manager->GetFileManager()->SaveFile(filepath + ".pkg", content);
}

std::string Package::SerializePackage(std::vector<GameObject*> goVec)
{
	Json::Value jsonPackage;

	for (auto obj : goVec)
	{
		Json::Value objectValue;

		objectValue["Name"] = obj->GetName();

		Json::Value transform;

		if (obj->GetTransform() != nullptr)
		{
			auto transf = obj->GetTransform();
			transform["Dynamic"] = transf->Dynamic;
			Json::Value position;
			position["x"] = transf->Position.x;
			position["y"] = transf->Position.y;
			position["z"] = transf->Position.z;
			Json::Value rotation;
			rotation["x"] = transf->Rotation.x;
			rotation["y"] = transf->Rotation.y;
			rotation["z"] = transf->Rotation.z;
			Json::Value scale;
			scale["x"] = transf->Scale.x;
			scale["y"] = transf->Scale.y;
			scale["z"] = transf->Scale.z;

			Json::Value texPos;
			texPos["x"] = transf->TexturePosition.x;
			texPos["y"] = transf->TexturePosition.y;
			texPos["z"] = transf->TexturePosition.z;
			Json::Value texRot;
			texRot["x"] = transf->TextureRotation.x;
			texRot["y"] = transf->TextureRotation.y;
			texRot["z"] = transf->TextureRotation.z;
			Json::Value texScale;
			texScale["x"] = transf->TextureScale.x;
			texScale["y"] = transf->TextureScale.y;
			texScale["z"] = transf->TextureScale.z;

			transform["Position"] = position;
			transform["Rotation"] = rotation;
			transform["Scale"] = scale;
			transform["TexPos"] = texPos;
			transform["TexRot"] = texRot;
			transform["TexScale"] = texScale;

			objectValue["Transform"] = transform;
		}

		Json::Value renderer2d;
		Json::Value renderer3d;

		if (obj->Get2DRenderer() != nullptr)
		{
			auto rend2D = obj->Get2DRenderer();
			renderer2d["Enabled"] = rend2D->Enabled;
			renderer2d["TexID"] = rend2D->Texture;
			renderer2d["ZOrder"] = rend2D->ZOrder;

			Json::Value color;
			color["r"] = rend2D->Color.r;
			color["g"] = rend2D->Color.g;
			color["b"] = rend2D->Color.b;
			color["a"] = rend2D->Color.a;

			renderer2d["Color"] = color;

			objectValue["Renderer2D"] = renderer2d;
		}
		else if (obj->GetRenderer() != nullptr)
		{
			auto renderer = obj->GetRenderer();

			renderer3d["Enabled"] = renderer->Enabled;
			renderer3d["EnabledBackCulling"] = renderer->EnableBackFaceCulling;
			renderer3d["CastShadow"] = renderer->CastShadow;

			Json::Value material;
			material["albedo"] = renderer->Material.albedo;
			material["normal"] = renderer->Material.normal;
			material["roughness"] = renderer->Material.roughness;
			material["metallic"] = renderer->Material.metallic;
			material["emissive"] = renderer->Material.emissive;

			renderer3d["Material"] = material;

			Json::Value model;
			model["ModelPath"] = renderer->ModelPath;
			model["PrimitiveType"] = renderer->PrimitiveType;
			model["ModelTypePrimitive"] = renderer->ModelTypePrimitive;

			renderer3d["Model"] = model;

			objectValue["Renderer3D"] = renderer3d;
		}

		jsonPackage[obj->GetName()] = objectValue;
	}

	return jsonPackage.toStyledString();
}

std::vector<GameObject*> Package::DeserializePackage(std::string packageData)
{
	Json::Value jsonPackage;
	Json::Reader reader;

	std::vector<GameObject*> m_objList;

	if (reader.parse(packageData, jsonPackage))
	{
		for (auto objVal : jsonPackage)
		{
			auto obj = m_manager->GetCurrentScene()->CreateObject();
			obj->SetName(objVal.get("Name", "").asString());

			auto transfVal = objVal["Transform"];
			if (transfVal.size() != 0)
			{
				auto transform = obj->AddComponent<Transform>();
				transform->Dynamic = transfVal.get("Dynamic", false).asBool();
				auto posVal = transfVal["Position"];
				transform->Position.x = posVal.get("x", 0.0f).asFloat();
				transform->Position.y = posVal.get("y", 0.0f).asFloat();
				transform->Position.z = posVal.get("z", 0.0f).asFloat();
				auto rotVal = transfVal["Rotation"];
				transform->Rotation.x = rotVal.get("x", 0.0f).asFloat();
				transform->Rotation.y = rotVal.get("y", 0.0f).asFloat();
				transform->Rotation.z = rotVal.get("z", 0.0f).asFloat();
				auto scalVal = transfVal["Scale"];
				transform->Scale.x = scalVal.get("x", 1.0f).asFloat();
				transform->Scale.y = scalVal.get("y", 1.0f).asFloat();
				transform->Scale.z = scalVal.get("z", 1.0f).asFloat();
				auto texPos = transfVal["TexPos"];
				transform->TexturePosition.x = texPos.get("x", 0.0f).asFloat();
				transform->TexturePosition.y = texPos.get("y", 0.0f).asFloat();
				transform->TexturePosition.z = texPos.get("z", 0.0f).asFloat();
				auto texRot = transfVal["TexRot"];
				transform->TextureRotation.x = texRot.get("x", 0.0f).asFloat();
				transform->TextureRotation.y = texRot.get("y", 0.0f).asFloat();
				transform->TextureRotation.z = texRot.get("z", 0.0f).asFloat();
				auto texSca = transfVal["TexScale"];
				transform->TextureScale.x = texSca.get("x", 1.0f).asFloat();
				transform->TextureScale.y = texSca.get("y", 1.0f).asFloat();
				transform->TextureScale.z = texSca.get("z", 1.0f).asFloat();
			}

			auto rend2dVal = objVal["Renderer2D"];
			if (rend2dVal.size() != 0)
			{
				auto renderer2d = obj->AddComponent<Renderer2D>();
				renderer2d->Enabled = rend2dVal.get("Enabled", true).asBool();
				renderer2d->Texture = rend2dVal.get("TexID", 0).asUInt();
				renderer2d->ZOrder = rend2dVal.get("ZOrder", 0).asInt();

				auto color = rend2dVal["Color"];
				renderer2d->Color.r = color.get("r", 1.0f).asFloat();
				renderer2d->Color.g = color.get("g", 1.0f).asFloat();
				renderer2d->Color.b = color.get("b", 1.0f).asFloat();
				renderer2d->Color.a = color.get("a", 1.0f).asFloat();
			}

			auto rend3dVal = objVal["Renderer3D"];
			if (rend3dVal.size() != 0)
			{
				auto renderer3d = obj->AddComponent<Renderer3D>();
				renderer3d->Enabled = rend3dVal.get("Enabled", true).asBool();
				renderer3d->EnableBackFaceCulling = rend3dVal.get("EnabledBackCulling", true).asBool();
				renderer3d->CastShadow = rend3dVal.get("CastShadow", true).asBool();
				auto matVal = rend3dVal["Material"];
				renderer3d->Material.albedo = matVal.get("albedo", 0).asUInt();
				renderer3d->Material.normal = matVal.get("normal", 0).asUInt();
				renderer3d->Material.metallic = matVal.get("metallic", 0).asUInt();
				renderer3d->Material.roughness = matVal.get("roughness", 0).asUInt();
				renderer3d->Material.emissive = matVal.get("emissive", 0).asUInt();
				auto modelVal = rend3dVal["Model"];
				renderer3d->ModelPath = modelVal.get("ModelPath", "").asString();
				renderer3d->ModelTypePrimitive = modelVal.get("ModelTypePrimitive", false).asBool();
				renderer3d->PrimitiveType = modelVal.get("PrimitiveType", 0).asUInt();

				if (renderer3d->ModelTypePrimitive)
				{
					renderer3d->Model = m_manager->GetModel()->AddGeometry(renderer3d->PrimitiveType);
				}
				else
				{
					renderer3d->Model = m_manager->GetModel()->AddModelFromFile(renderer3d->ModelPath);
				}
			}

			m_objList.push_back(obj);
		}
	}

	return m_objList;
}
