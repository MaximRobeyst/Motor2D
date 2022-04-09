#include "MiniginPCH.h"
#include "FoodComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "imgui.h"

FoodComponent::FoodComponent(dae::GameObject* pGameobject)
	: Component(pGameobject)
	, m_pTransform(pGameobject->GetComponent<dae::TransformComponent>())
	, m_pSubject(std::make_unique<Subject>())
{
}

void FoodComponent::Update()
{
	if (m_Falling)
	{
		m_pTransform->SetPosition(m_pTransform->GetPosition() + fallingSpeed);
	}
}
#ifdef _DEBUG
void FoodComponent::RenderGUI()
{
	ImGui::Text("I am a foodcomponent");
}
#endif // _DEBUG


void FoodComponent::SetFalling(bool newValue)
{
	m_Falling = newValue;
	if (m_Falling)
		m_pSubject->Notify(*m_pGameObject, Event::Burger_Drop);
}

std::unique_ptr<Subject>& FoodComponent::GetSubject()
{
	return m_pSubject;
}
