#include "GameManagerComponent.h"
#include <GameObject.h>
#include <Subject.h>
#include "Event.h"
#include "PlayerComponent.h"

GameManagerComponent::GameManagerComponent(dae::GameObject* pGameobject, dae::GameObject* pPlayer, int burgerAmount)
	: dae::Component(pGameobject)
	, m_pPlayerObject{pPlayer}
	, m_AmountOfBurgers{ burgerAmount }
{
}

void GameManagerComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& action)
{
	switch (action)
	{
		case Event::Burger_Made:
			m_pPlayerObject->GetComponent<PlayerComponent>();
			std::cout << "Won!" << std::endl;
			break;
	}
}
