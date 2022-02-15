#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "GameObject.h"
#include "TextObject.h"
#include "Time.h"

dae::FPSComponent::FPSComponent(dae::GameObject* pGameObject)
	:Component{pGameObject}
{
}

void dae::FPSComponent::Update()
{
	int fps = static_cast<int>(1.f / (1.f / Time::GetInstance()->GetElapsed()));

	m_pGameObject->GetComponent<TextComponent>()->SetText(std::to_string(fps) + " FPS");
}

void dae::FPSComponent::Render() const
{
}
