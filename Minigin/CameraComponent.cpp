#include "MiniginPCH.h"
#include "CameraComponent.h"

#include "GameObject.h"
#include "Transform.h"
#include "GameTime.h"
#include "Renderer.h"
#include "InputManager.h"
#include <gl/GL.h>

dae::CameraComponent* dae::CameraComponent::m_MainCamera{nullptr};

dae::CameraComponent::CameraComponent()
	: m_Width{ static_cast<float>(dae::Renderer::GetInstance().GetWindowWidth()) }
	, m_Height{ static_cast<float>(dae::Renderer::GetInstance().GetWindowHeight()) }

{
}

dae::CameraComponent::CameraComponent(GameObject* pGameobject)
	: Component{pGameobject}
	, m_Width{ static_cast<float>(dae::Renderer::GetInstance().GetWindowWidth()) }
	, m_Height{ static_cast<float>(dae::Renderer::GetInstance().GetWindowHeight())}
{
}

dae::CameraComponent::~CameraComponent()
{
	if (dae::CameraComponent::m_MainCamera == this)
		dae::CameraComponent::m_MainCamera = nullptr;
}

void dae::CameraComponent::Start()
{
	m_pTransformComponent = m_pGameObject->GetComponent<TransformComponent>();
	if (m_MainCamera == nullptr) m_MainCamera = this;


}

void dae::CameraComponent::Update()
{
	if (m_pTarget == nullptr) return;

	auto& transform = m_pTransformComponent->GetTransform();
	glm::vec3 target{ glm::vec3{Track(m_pTarget->GetPosition()), 0} };

	transform.position += (target - transform.position) * m_Speed * GameTime::GetInstance()->GetElapsed();
}

void dae::CameraComponent::Render() const
{
	glScalef(m_pTransformComponent->GetScale().x, m_pTransformComponent->GetScale().y, 0);
	glTranslatef(-m_pTransformComponent->GetPosition().x, -m_pTransformComponent->GetPosition().y, 0);

	dae::Renderer::GetInstance().RenderCircle(WindowToGameWorld(dae::InputManager::GetInstance().GetMousePosition()), 1.0f);
}

void dae::CameraComponent::RenderGUI()
{
}

void dae::CameraComponent::SetTarget(TransformComponent* target)
{
	m_pTarget = target;
}

glm::vec3 dae::CameraComponent::WindowToGameWorld(glm::vec2 point) const
{
	auto cameraPos = m_pTransformComponent->GetPosition();;

	return glm::vec3{ (point.x / m_pTransformComponent->GetScale().x) + cameraPos.x, (point.y / m_pTransformComponent->GetScale().y) + cameraPos.y , 0 };
}

dae::CameraComponent* dae::CameraComponent::GetMainCamera()
{
	return m_MainCamera;
}

glm::vec2 dae::CameraComponent::Track(const glm::vec2& target)
{
	auto& transform = m_pTransformComponent->GetTransform();
	return glm::vec2{ (target.x) - ((m_Width / transform.scale.x) / 2), (target.y) - ((m_Height / transform.scale.y) / 2) };
}
