#pragma once
#include <Component.h>
#include <Observer.h>

class GameManagerComponent final : public dae::Component, public Observer
{
public:
	GameManagerComponent() = default;
	GameManagerComponent(dae::GameObject * pGameobject, dae::GameObject* pPlayer, int burgerAmount = 4);

	virtual void Notify(const dae::GameObject& gameObject, const Event& action) override;

private:
	int m_AmountOfBurgers;
	dae::GameObject* m_pPlayerObject{};
};

