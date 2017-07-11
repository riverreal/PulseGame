#include "SpriteNumber.h"
#include"../Source/Includes/LESystem.h"
using namespace Elixir;

void SpriteNumber::Initialize(SceneManager * sceneManager)
{
	Manager = sceneManager;

	m_parentsObj = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
	m_parentsObj->Get2DRenderer()->Color.a = 0;
	m_parentsObj->SetName("SpriteNumParent");
}

void SpriteNumber::UpdateSprite(int value)
{
	if (value < 0)return;

	int digit = value;
	vector<int> number;
	
	if (value == 0)
	{
		number.push_back(value);
	}
	else
	{
		while (digit != 0)
		{
			value = digit % 10;
			digit /= 10;
			number.push_back(value);
		}
	}

	int childrenVecSize = m_parentsObj->GetChildren().size();

	if (childrenVecSize < number.size())
	{
		for (int i = 0; i < number.size() - childrenVecSize; i++)
		{
			auto child = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_2D);
			m_parentsObj->AddChild(child);
			
		}
	}
	else if (childrenVecSize > number.size())
	{
		int digitToRemove = childrenVecSize - number.size();

		for (int i = 0; i < digitToRemove; i++)
		{
			m_parentsObj->GetChildren()[childrenVecSize - (i + 1)]->Get2DRenderer()->Enabled = false;
			//Manager->GetCurrentScene()->RemoveObject(m_parentsObj->GetChildren()[childrenVecSize - (i + 1)]);
		}


	}

	float spriteScaleX = m_parentsObj->GetTransform()->Scale.x;

	float digitPosValue = (float)number.size() - ((number.size() / 2) + ((float)(number.size() % 2) * 0.5f) + 0.5f);
	
	for (int i = 0; i < number.size(); i++)
	{
		m_parentsObj->GetChildren()[i]->Get2DRenderer()->Enabled = true;
		m_parentsObj->GetChildren()[i]->GetTransform()->Position = m_parentsObj->GetTransform()->Position;

		m_parentsObj->GetChildren()[i]->Get2DRenderer()->Texture =
			Manager->GetTextureManager()->AddTexture(m_spritePath[number[i]]);

		m_parentsObj->GetChildren()[i]->GetTransform()->Position.x +=
			(digitPosValue - (float)i) * 170 * spriteScaleX;

		m_parentsObj->GetChildren()[i]->Get2DRenderer()->Color = m_parentsObj->Get2DRenderer()->Color;
		m_parentsObj->GetChildren()[i]->Get2DRenderer()->Color.a = 1;
		m_parentsObj->GetChildren()[i]->GetTransform()->Scale = m_parentsObj->GetTransform()->Scale;
			
	}

}

GameObject* SpriteNumber::GetSpriteParent()
{
	return m_parentsObj;
}