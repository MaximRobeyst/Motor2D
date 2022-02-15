#pragma once
#include "Component.h"

namespace dae
{
	class Texture2D;

	class SpriteRendererComponent final : public Component
	{
	public:
		explicit SpriteRendererComponent(dae::GameObject* pGameObject, const std::string& spritePath);
		~SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent(SpriteRendererComponent&& other) = delete;
		SpriteRendererComponent& operator=(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent& operator=(SpriteRendererComponent&& other) = delete;

		void SetTexture(const std::shared_ptr<Texture2D>& texture);

		void Update() override;
		void Render() const override;
	private:
		// member functions


		// member variables
		std::shared_ptr<Texture2D> m_pTexture;
	};
}
