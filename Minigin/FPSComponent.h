#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;
	class FPSComponent final :public Component
	{
	public:
		FPSComponent() = default;
		explicit FPSComponent(dae::GameObject* pGameObject);
		~FPSComponent() = default;
		FPSComponent(const FPSComponent & other) = delete;
		FPSComponent(FPSComponent && other) = delete;
		FPSComponent& operator=(const FPSComponent & other) = delete;
		FPSComponent& operator=(FPSComponent && other) = delete;

		void Start() override;
		void Update() override;
		void Render() const override;

	private:
		TextComponent* m_pTextComponent{};
	};
}


