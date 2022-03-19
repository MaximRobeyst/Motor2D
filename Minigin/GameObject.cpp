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

void dae::GameObject::SetParent(GameObject* pParent, bool /*worldPositionStays*/)
{
	if (m_pParent != nullptr)
	{
		pParent->RemoveChild(this);
	}
	m_pParent = pParent;
	if(m_pParent != nullptr)
		pParent->AddChild(this);
	// Update position

	// if already has a parent remove if from the other parent
	// Set transform relative (Check integrity of scenegraph)
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	if (m_pParent == nullptr)
		return;

	if (pChild == m_pParent)
		return;

	if (GameObject* pChildParent = pChild->GetParent(); pChildParent != nullptr)
	{
		pChildParent->RemoveChild(pChild);
	}

	pChild->SetParent(this);
	m_pChildren.push_back(pChild);

	// if already has a parent remove if from the other parent
	// Set transform relative (Check integrity of scenegraph)

}

void dae::GameObject::RemoveChild(GameObject* pGameObject)
{
	auto gameObject = std::find(m_pChildren.begin(), m_pChildren.end(), pGameObject);
	std::swap(*gameObject, m_pChildren[m_pChildren.size() - 1]);
	m_pChildren.pop_back();

	pGameObject->SetParent(nullptr);

	// Update Position
}

dae::GameObject* dae::GameObject::GetChildFromIndex(int i) const
{
	assert(i < static_cast<int>(m_pChildren.size()));

	return m_pChildren[i];
}

size_t dae::GameObject::GetAmountOfChildren() const
{
	return m_pChildren.size();
}
