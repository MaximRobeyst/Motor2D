#pragma once
#include "Transform.h"
#include "RigidbodyComponent.h"

class Command 
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;

	virtual void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>&) {};
	virtual void Deserialize(rapidjson::Value&) {};
};

class FoodComponent;
class FallCommand : public Command
{
public:
	FallCommand(FoodComponent* pFoodComponent);
	void Execute();

private:
	FoodComponent* m_pFoodComponent;
};

class MoveCommand : public Command
{
public:
	MoveCommand(dae::RigidbodyComponent* pTransformComponent, glm::vec2 movement, float speed = 10.f);
	void Execute();

private:
	glm::vec2 m_Movement{};
	dae::RigidbodyComponent* m_pRigidbody{};
	float m_Speed{};
};


