#pragma once
#include "Transform.h"

class Command 
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};

class FireCommand : public Command
{
public:
	void Execute() override;
};

class DuckCommand : public Command
{
public:
	void Execute() override;
};

class JumpCommand : public Command
{
public:
	void Execute() override;
};

class FartCommand : public Command
{
public:
	void Execute() override;
};

class LifeComponent;
class KillCommand : public Command
{
public:
	KillCommand(LifeComponent* pHealthTarget);
	void Execute() override;

private:
	LifeComponent* m_pLifeTarget;
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
	MoveCommand(dae::TransformComponent* pTransformComponent, glm::vec3 movement, float speed = 10.f);
	void Execute();

private:
	glm::vec3 m_Movement{};
	dae::TransformComponent* m_pTransform{};
	float m_Speed{};
};


