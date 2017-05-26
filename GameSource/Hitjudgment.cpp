#include "Hitjudgment.h"
using namespace Elixir;

bool Hitjudgment::AABBColliding(Elixir::GameObject * obj1, Elixir::GameObject * obj2)
{
	auto box1Center = obj1->GetTransform()->Position;
	auto box1Size = obj1->GetTransform()->Scale;
	auto box2Center = obj2->GetTransform()->Position;
	auto box2Size = obj2->GetTransform()->Scale;

	
	Vec3f box1Min;
	box1Min.x = box1Center.x - box1Size.x / 2;
	box1Min.y = box1Center.y - box1Size.y / 2;
	box1Min.z = box1Center.z - box1Size.z / 2;
	Vec3f box1Max;
	box1Max.x = box1Center.x + box1Size.x / 2;
	box1Max.y = box1Center.y + box1Size.y / 2;
	box1Max.z = box1Center.z + box1Size.z / 2;
	Vec3f box2Min;
	box2Min.x = box2Center.x - box2Size.x / 2;
	box2Min.y = box2Center.y - box2Size.y / 2;
	box2Min.z = box2Center.z - box2Size.z / 2;
	Vec3f box2Max;
	box2Max.x = box2Center.x + box2Size.x / 2;
	box2Max.y = box2Center.y + box2Size.y / 2;
	box2Max.z = box2Center.z + box2Size.z / 2;

	return(box1Max.x > box2Min.x &&
		box1Min.x < box2Max.x&&
		box1Max.y > box2Min.y&&
		box1Min.y < box2Max.y&&
		box1Max.z > box2Min.z&&
		box1Min.z < box2Max.z);
	
	
}

bool Hitjudgment::SpColliding(GameObject* obj1, GameObject* obj2)
{
	auto delta = obj2->GetTransform()->Position - obj1->GetTransform()->Position;
	float deltaSq = delta.Dot(delta);

	float avrgSca = obj1->GetTransform()->Scale.x + obj1->GetTransform()->Scale.y + obj1->GetTransform()->Scale.z;
	avrgSca /= 5.7f;

	float radiiSumSq = avrgSca + avrgSca;
	radiiSumSq *= radiiSumSq;

	if (deltaSq <= radiiSumSq)
	{
		return true;
	}

	return false;
}