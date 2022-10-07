#include "WeaponComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Collider.h"
#include "PlayerComponent.h"
#include "InteractComponent.h"
#include "Transform.h"
#include "ProjectileComponent.h"
#include "GameTime.h"
#include "TextObject.h"

#include <Scene.h>
#include <Renderer.h>
#include <Utils.h>

WeaponComponent::WeaponComponent()
{
}

WeaponComponent::WeaponComponent(int ammo, int shellPieces, float maxOffsetAngle, float barrelLength, float fireRate)
	: m_Ammo{ammo}
	, m_ShellPieces{shellPieces}
	, m_MaxOffsetAngle{maxOffsetAngle}
	, m_BarrelLength{barrelLength}
	, m_FireRate{fireRate}
{
}

void WeaponComponent::Start()
{
	m_pTransformComponent = m_pGameObject->GetComponent<dae::TransformComponent>();
	m_pPlayerComponent = m_pGameObject->GetComponent<PlayerComponent>();

	m_pTextComponent = m_pGameObject->GetParent()->GetComponentInChildren<dae::TextComponent>();
}

void WeaponComponent::Update()
{
	m_WaitTillNextShot -= GameTime::GetInstance()->GetElapsed() * m_FireRate;
}

void WeaponComponent::Render() const
{
	//glm::vec2 position = m_pTransformComponent->GetPosition();
	//float angle = m_pTransformComponent->GetRotation();
	//
	//float lineLength{ 10 };
	//float triangleAngles = (static_cast<float>(M_PI) / 6);
	//static std::vector<glm::vec2> points;
	//points.resize(3);
	//points.push_back( { position.x + cosf(angle) * m_BarrelLength, position.y + sinf(angle) * m_BarrelLength});
	//points.push_back( { position.x - cosf(angle - triangleAngles) * lineLength, position.y - sinf(angle - triangleAngles) * lineLength });
	//points.push_back( { position.x - cosf(angle + triangleAngles) * lineLength, position.y - sinf(angle + triangleAngles) * lineLength });
	//
	//dae::Renderer::GetInstance().RenderPolygon(points.data(), 3, SDL_Color{ 255, 255, 255, 255 });
}

void WeaponComponent::Attack()
{
	if (m_WaitTillNextShot > 0.0f) return;
	//if (m_Ammo <= 0) return;

	for (int i = 0; i < m_ShellPieces; ++i)
	{
		auto pProjectile = new dae::GameObject("Bullet" + std::to_string(i));
		pProjectile->SetTag("Projectile");

		pProjectile->AddComponent(new dae::TransformComponent(pProjectile, m_pTransformComponent->GetPosition()));
		pProjectile->GetTransform()->SetRotation(m_pTransformComponent->GetRotation() + Random(-m_MaxOffsetAngle, m_MaxOffsetAngle));
		pProjectile->AddComponent(new dae::SpriteRendererComponent(pProjectile, "Sprites/Arrow.png"));
		pProjectile->AddComponent(new dae::ColliderComponent(pProjectile));
		pProjectile->AddComponent(new dae::RigidbodyComponent(pProjectile, b2_dynamicBody, 1.0f, 1.0f, false));
		pProjectile->AddComponent(new ProjectileComponent());

		GetGameObject()->GetScene()->AddGameObject(pProjectile);
	}

	--m_Ammo;

}
