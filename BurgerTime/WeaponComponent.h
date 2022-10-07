#pragma once
#include <Component.h>

class InteractComponent;
class PlayerComponent;

namespace dae
{
	class RigidbodyComponent;
	class TransformComponent;
	class TextComponent;
}

class WeaponComponent : public dae::Component
{
public:
	WeaponComponent();
	WeaponComponent(int ammo, int shellPieces, float maxOffsetAngle, float barrelLength, float fireRate);

	void Start() override;
	void Update() override;
	void Render() const override;

	void Attack();		// Shoot function


private:
	dae::TransformComponent* m_pTransformComponent;
	dae::TextComponent* m_pTextComponent;
	PlayerComponent* m_pPlayerComponent;

	int m_Ammo{};
	int m_ShellPieces{};
	
	float m_MaxOffsetAngle{};
	float m_BarrelLength{};
	float m_FireRate{};

	float m_Angle{};
	float m_WaitTillNextShot{ 1.0f };
};

