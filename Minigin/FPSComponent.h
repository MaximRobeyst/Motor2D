#pragma once
#include "Component.h"

namespace dae
{
	class FPSComponent final :public Component
	{
	public:
		explicit FPSComponent(dae::GameObject* pGameObject);
		~FPSComponent() = default;
		FPSComponent(const FPSComponent & other) = delete;
		FPSComponent(FPSComponent && other) = delete;
		FPSComponent& operator=(const FPSComponent & other) = delete;
		FPSComponent& operator=(FPSComponent && other) = delete;

		void Update() override;
		void Render() const override;
	};
}


