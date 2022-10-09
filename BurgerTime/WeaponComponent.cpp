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

	m_pTextComponent->SetText(std::to_string(m_Ammo));
}

void WeaponComponent::Update()
{
	m_WaitTillNextShot -= GameTime::GetInstance()->GetElapsed() * m_FireRate;
}

void WeaponComponent::Render() const
{
}

void WeaponComponent::Attack()
{
	if (m_WaitTillNextShot > 0.0f) return;
	if (m_Ammo <= 0) return;

	for (int i = 0; i < m_ShellPieces; ++i)
	{
		auto pProjectile = new dae::GameObject("Bullet" + std::to_string(i));
		pProjectile->SetTag("Projectile");

		pProjectile->AddComponent(new dae::TransformComponent(pProjectile, m_pTransformComponent->GetPosition()));
		pProjectile->GetTransform()->SetRotation(m_pTransformComponent->GetRotation() + Random(-m_MaxOffsetAngle, m_MaxOffsetAngle));
		pProjectile->AddComponent(new dae::SpriteRendererComponent(pProjectile, "Sprites/Arrow.png"));
		pProjectile->AddComponent(new dae::ColliderComponent(pProjectile));


		uint16 mask = 0;
		mask |= static_cast<uint16>(dae::PhysicsLayers::DefaultLayer);
		mask |= static_cast<uint16>(dae::PhysicsLayers::layer2);
		mask |= static_cast<uint16>(dae::PhysicsLayers::layer3);
		pProjectile->AddComponent(new dae::RigidbodyComponent(pProjectile, b2_dynamicBody, 1.0f, 1.0f, false, dae::PhysicsLayers::layer1, mask));
		pProjectile->AddComponent(new ProjectileComponent());

		GetGameObject()->GetScene()->AddGameObject(pProjectile);
	}


	--m_Ammo;
	m_WaitTillNextShot = 1.0f;
	if (m_Ammo <= 0)
	{
		SpawnPickup();
	}

	m_pTextComponent->SetText(std::to_string(m_Ammo));

}

void WeaponComponent::SpawnPickup()
{
	auto pGameobject = new dae::GameObject();
	pGameobject->SetTag("Pickup");
	pGameobject->AddComponent(new dae::TransformComponent(pGameobject, glm::vec3{
		Random(50.f, dae::Renderer::GetInstance().GetWindowWidth() - 50.f),
		Random(50.f, dae::Renderer::GetInstance().GetWindowHeight() - 50.f),0 },
		glm::vec2{0.25f}));

	pGameobject->AddComponent(new dae::SpriteRendererComponent(pGameobject, "Sprites/pickup.png", SDL_FRect{ 0, 255, 0, 255 }));
	pGameobject->AddComponent(new dae::ColliderComponent(pGameobject));
	auto pRigidbody = new dae::RigidbodyComponent(pGameobject, b2_dynamicBody, 1.0f, 1.0f, true);
	pGameobject->AddComponent(pRigidbody);


	std::function<void(dae::RigidbodyComponent*, dae::RigidbodyComponent*, b2Contact*)> newFunction = [this](dae::RigidbodyComponent* pTriggeredbody, dae::RigidbodyComponent* otherBody, b2Contact*)
	{
		if (otherBody->GetGameObject()->GetTag() == "Player")
		{
			m_Ammo = Random(5, 20);
			m_ShellPieces = Random(1, 10);
			m_MaxOffsetAngle = Random(0.0f, static_cast<float>(M_PI / 2));
			m_BarrelLength = 10;
			m_FireRate = Random(1.0f, 5.0f);


			m_pTextComponent->SetText(std::to_string(m_Ammo));

			dae::GameObject::Destroy(pTriggeredbody->GetGameObject());
		}
	};

	pRigidbody->SetOnEnterFunction(newFunction);


	m_pGameObject->GetScene()->AddGameObject(pGameobject);
}
