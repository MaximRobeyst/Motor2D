#include "LifeComponent.h"
#include "GameObject.h"
#include "Subject.h"
#include "Event.h"

#include <Observer.h>
#include <Scene.h>

const dae::Creator<dae::Component, LifeComponent> g_LifeComponentCreator{};

LifeComponent::LifeComponent()
	: m_pSubject{std::make_unique<Subject>()}
{
}

LifeComponent::LifeComponent(dae::GameObject* pGameObject, int lives)
	: dae::Component(pGameObject)
	, m_Lives{lives}
	, m_pSubject{ std::make_unique<Subject>() }
{
}

void LifeComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());
	writer.Key("Lives");
	writer.Int(m_Lives);/*
	writer.Key("Observers");
	writer.StartArray();
	for (int i = 0; i < m_pSubject->GetNrOfObservers(); ++i)
	{
		writer.Int(m_pSubject->GetObserverFromId(i)->GetId());
	}
	writer.EndArray();*/
	writer.EndObject();
}

void LifeComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
	m_Lives = value["Lives"].GetInt();

	//for (auto iter = value["Observers"].Begin(); iter != value["Observers"].End(); ++iter)
	//{
	//	auto pObserver = pGameobject->GetScene()->GetGameobjectFromId(iter->GetInt());

	//	m_pSubject->AddObserver(pObserver->GetComponent<Observer>());

	//}
}

void LifeComponent::Hit()
{
	if (m_Lives == 0 || !m_Enabled) return;

	--m_Lives;

	m_pSubject->Notify(*m_pGameObject, Event::Player_Died);
}

int LifeComponent::GetLives() const
{
	return m_Lives;
}

std::unique_ptr<Subject>& LifeComponent::GetSubject()
{
	return m_pSubject;
}

void LifeComponent::SetEnabled(bool newBool)
{
	m_Enabled = newBool;
}
