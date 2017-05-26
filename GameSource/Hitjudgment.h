#pragma once
#include"../Source/System/GameObject.h"
class Hitjudgment
{
public:
	static bool AABBColliding(Elixir::GameObject* obj1,Elixir::GameObject* obj2);
	static bool SpColliding(Elixir::GameObject* obj1, Elixir::GameObject* obj2);
	

private:

};

