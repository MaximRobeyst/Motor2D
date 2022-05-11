#include "MiniginPCH.h"
#include "Component.h"

void dae::Component::SetGameObject(GameObject* pGameObject)
{
	if (m_pGameObject != nullptr)
	{
		// different logic
		return;
	}

	m_pGameObject = pGameObject;
}
