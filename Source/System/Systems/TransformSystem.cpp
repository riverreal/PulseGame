#include "TransformSystem.h"
#include <DirectXMath.h>

using namespace DirectX;

void Elixir::TransformSystem::Initialize(GameObject* object)
{
	if (CheckDependancies(object))
	{
		auto transform = object->GetTransform();

		//Don't check whether it's dynamic or not, just update
		UpdateMatrices(transform);
	}
}

void Elixir::TransformSystem::Update(GameObject * object, F32 deltaTime)
{
	if (CheckDependancies(object))
	{
		auto transform = object->GetTransform();

		//Check if it's dynamic
		if (transform->Dynamic)
		{
			transform->AbsolutePosition = transform->Position;
			transform->AbsoluteRotation = transform->Rotation;
			transform->AbsoluteScale = transform->Scale;

			auto parent = object->GetParent();

			//inherit transform informaiton from parent
			if (parent != nullptr)
			{
				auto pTransform = parent->GetTransform();
				if (pTransform != nullptr)
				{
					transform->AbsolutePosition = transform->AbsolutePosition + pTransform->AbsolutePosition;
					transform->AbsoluteRotation = transform->AbsoluteRotation + pTransform->AbsoluteRotation;
					transform->AbsoluteScale = transform->AbsoluteScale + pTransform->AbsoluteScale;
				}
			}

			//transform->AbsolutePosition = transform->Position + object
			UpdateMatrices(transform);
		}
	}
}

bool Elixir::TransformSystem::CheckDependancies(GameObject * object)
{
	bool check = true;
	
	if (object->GetTransform() == nullptr)
	{
		check = false;
	}

	return check;
}

void Elixir::TransformSystem::UpdateMatrices(Transform* transform)
{
	//object world Update
	XMMATRIX scaling = XMMatrixScaling(transform->AbsoluteScale.x, transform->AbsoluteScale.y, transform->AbsoluteScale.z);
	XMMATRIX rotation = XMMatrixMultiply(XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(transform->AbsoluteRotation.y)),
		XMMatrixRotationX(XMConvertToRadians(transform->AbsoluteRotation.x))), XMMatrixRotationZ(XMConvertToRadians(transform->AbsoluteRotation.z)));
	XMMATRIX translation = XMMatrixTranslation(transform->AbsolutePosition.x, transform->AbsolutePosition.y, transform->AbsolutePosition.z);
	auto WorldMatrix = XMMatrixMultiply(XMMatrixMultiply(scaling, rotation), translation);

	XMStoreFloat4x4(&transform->World4x4, WorldMatrix);

	//tex transforma Update
	scaling = XMMatrixScaling(transform->TextureScale.x, transform->TextureScale.y, transform->TextureScale.z);
	rotation = XMMatrixMultiply(XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(transform->TextureRotation.y)),
		XMMatrixRotationX(XMConvertToRadians(transform->TextureRotation.x))), XMMatrixRotationZ(XMConvertToRadians(transform->TextureRotation.z)));
	translation = XMMatrixTranslation(transform->TexturePosition.x, transform->TexturePosition.y, transform->TexturePosition.z);
	auto TexMatrix = XMMatrixMultiply(XMMatrixMultiply(scaling, rotation), translation);

	XMStoreFloat4x4(&transform->TextureTransform4x4, TexMatrix);
}
