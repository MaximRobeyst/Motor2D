#include "MiniginPCH.h"
#include "Command.h"

#include "LifeComponent.h"
#include "FoodComponent.h"
#include "Time.h"

void FireCommand::Execute()
{
	std::cout << "Fire" << std::endl;
}

void DuckCommand::Execute()
{
	std::cout << "Dock" << std::endl;
}

void JumpCommand::Execute()
{
	std::cout << "Jump" << std::endl;
}

void FartCommand::Execute()
{
	std::cout << "Fart" << std::endl;
}

KillCommand::KillCommand(LifeComponent* pLifeTarget)
	:m_pLifeTarget{ pLifeTarget }
{
}

void KillCommand::Execute()
{
	m_pLifeTarget->Hit();
}

FallCommand::FallCommand(FoodComponent* pFoodComponent)
	: m_pFoodComponent(pFoodComponent)
{
}

void FallCommand::Execute()
{
	m_pFoodComponent->SetFalling(true);
}

MoveCommand::MoveCommand(dae::TransformComponent* pTransformComponent, glm::vec3 movement, float speed)
	: m_pTransform{pTransformComponent}
	, m_Movement{movement}
	, m_Speed{speed}
{
}

void MoveCommand::Execute()
{
	m_pTransform->Move(m_Movement * m_Speed * Time::GetInstance()->GetElapsed());
}
