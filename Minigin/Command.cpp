#include "MiniginPCH.h"
#include "Command.h"

//#include "LifeComponent.h"
//#include "FoodComponent.h"

#include "Time.h"
//KillCommand::KillCommand(LifeComponent* pLifeTarget)
//	:m_pLifeTarget{ pLifeTarget }
//{
//}
//
//void KillCommand::Execute()
//{
//	//m_pLifeTarget->Hit();
//}

FallCommand::FallCommand(FoodComponent* pFoodComponent)
	: m_pFoodComponent(pFoodComponent)
{
}

void FallCommand::Execute()
{
	//m_pFoodComponent->SetFalling(true);
}

MoveCommand::MoveCommand(dae::RigidbodyComponent* pTransformComponent, glm::vec2 movement, float speed)
	: m_pRigidbody{pTransformComponent}
	, m_Movement{movement}
	, m_Speed{speed}
{
}

void MoveCommand::Execute()
{
	//m_pRigidbody->SetVelocity(m_Movement * m_Speed /** Time::GetInstance()->GetElapsed()*/);
}

void Command::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());
	writer.EndObject();
}
