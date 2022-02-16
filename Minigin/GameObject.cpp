#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject(GameObject* pParent, std::vector<GameObject*> pChildren)
	: m_pParent{pParent}
	, m_pChildren{pChildren}
{
	pParent->AddChild(this);
}

dae::GameObject::~GameObject()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
		delete *iter;
	m_pComponents.clear();


	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
		delete* iter;
	m_pChildren.clear();
}

void dae::GameObject::Update()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Update();
	}

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
		(*iter)->Update();
}

void dae::GameObject::Render() const
{
	//const auto& pos = m_Transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Render();
	}

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
		(*iter)->Render();

}

void dae::GameObject::AddComponent(Component* component)
{
	m_pComponents.push_back(component);
}

void dae::GameObject::SetParent(GameObject* pParent)
{
	pParent->AddChild(this);
	m_pParent = nullptr;
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.push_back(pChild);
}

dae::GameObject* dae::GameObject::GetChildFromIndex(int i) const
{
	assert(i < static_cast<int>(m_pChildren.size()));

	return m_pChildren[i];
}

std::vector<dae::GameObject*> dae::GameObject::GetChildren() const
{
	return m_pChildren;
}

size_t dae::GameObject::GetAmountOfChildren() const
{
	return m_pChildren.size();
}
