#pragma once
#include <Component.h>

class MrHotDogComponent : public dae::Component
{
public:
	MrHotDogComponent(dae::GameObject* pGameobject);
	~MrHotDogComponent();

	void Update() override;
};

