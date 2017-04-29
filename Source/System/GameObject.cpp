#include "GameObject.h"


using namespace Elixir;

GameObject::GameObject()
	:m_renderer(nullptr),
	m_transform(nullptr),
	m_2dRenderer(nullptr),
	m_children(0),
	m_name("NewGameObject")
{
}

GameObject::~GameObject()
{
}

void GameObject::AddComponent(IComponent* component)
{
	m_components[&typeid(*component)] = component;

	component->Initialize();

	bool registered = false;

	if (m_transform == nullptr)
	{
		auto trans = GetComponent<Transform>();
		if (trans != nullptr)
		{
			m_transform = trans;
			registered = true;
		}
	}

	if(m_renderer == nullptr && !registered)
	{
		auto rend = GetComponent<Renderer3D>();
		if (rend != nullptr)
		{
			m_renderer = rend;
			registered = true;
		}
	}

	if (m_2dRenderer == nullptr && !registered)
	{
		auto rend = GetComponent<Renderer2D>();
		if (rend != nullptr)
		{
			m_2dRenderer = rend;
			registered = true;
		}
	}
}

void GameObject::SetName(std::string name)
{
	m_name = name;
}

std::string GameObject::GetName()
{
	return m_name;
}

void GameObject::SetTag(I32 tag)
{
	m_tag = tag;
}

I32 GameObject::GetTag()
{
	return m_tag;
}

Transform* GameObject::GetTransform()
{
	return m_transform;
}

Renderer3D* GameObject::GetRenderer()
{
	return m_renderer;
}

Renderer2D * Elixir::GameObject::Get2DRenderer()
{
	return m_2dRenderer;
}

std::vector<GameObject*> GameObject::GetChildren()
{
	return m_children;
}

void GameObject::AddChild(GameObject* child)
{
	m_children.push_back(child);
}