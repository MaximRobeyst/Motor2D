#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "GameObject.h"
#include "TextObject.h"
#include "GameTime.h"

dae::Creator<dae::Component, dae::FPSComponent> g_FPSCreator{};

dae::FPSComponent::FPSComponent(dae::GameObject* pGameObject)
	:Component{pGameObject}
{
}

void dae::FPSComponent::Start()
{
	m_pTextComponent = m_pGameObject->GetComponent<TextComponent>();
}

void dae::FPSComponent::Update()
{
	m_pTextComponent->SetText(std::to_string(GameTime::GetInstance()->GetFPS()) + " FPS");
}

void dae::FPSComponent::Render() const
{
}
