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

	m_pChildren.clear();
}

void dae::GameObject::Start()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Start();
	}
}

void dae::GameObject::Update()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Update();
	}
}

void dae::GameObject::Render() const
{
	//const auto& pos = m_Transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->Render();
	}

}

#ifdef _DEBUG
void dae::GameObject::RenderGUI()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		(*iter)->RenderGUI();
	}
}
#endif // _DEBUG



void dae::GameObject::AddComponent(Component* component)
{
	m_pComponents.push_back(component);
}

void dae::GameObject::SetParent(GameObject* pParent, bool /*worldPositionStays*/)
{
	// Remove itself as a child from the previous parent
	if (m_pParent != nullptr)
		m_pParent->RemoveChild(this);

	// Set The given parent on itself
	m_pParent = pParent;

	// Add itself as a child to the given parent
	if(pParent != nullptr)
		pParent->AddChild(this);

	// TODO: Update position transform
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.push_back(pChild);

	// if already has a parent remove if from the other parent
	// Set transform relative (Check integrity of scenegraph)

}

void dae::GameObject::RemoveChild(GameObject* pGameObject)
{
	auto gameObject = std::find(m_pChildren.begin(), m_pChildren.end(), pGameObject);
	std::swap(*gameObject, m_pChildren[m_pChildren.size() - 1]);
	m_pChildren.pop_back();

	//pGameObject->SetParent(nullptr);

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

void dae::GameObject::SetTag(std::string tag)
{
	m_Tag = tag;
}

std::string dae::GameObject::GetTag() const
{
	return m_Tag;
}
