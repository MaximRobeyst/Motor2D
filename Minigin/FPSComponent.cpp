#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "GameObject.h"
#include "TextObject.h"
#include "GameTime.h"

dae::FPSComponent::FPSComponent(dae::GameObject* pGameObject)
	:Component{pGameObject}
{
	m_pTextCompoent = m_pGameObject->GetComponent<TextComponent>();
}

void dae::FPSComponent::Update()
{
	m_pTextCompoent->SetText(std::to_string(GameTime::GetInstance()->GetFPS()) + " FPS");
}

void dae::FPSComponent::Render() const
{
}
