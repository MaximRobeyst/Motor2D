#pragma once
#include "Component.h"

namespace dae
{
	class Texture2D;
	class TransformComponent;

	class SpriteRendererComponent final : public Component
	{
	public:
		explicit SpriteRendererComponent(GameObject* pGameObject, const std::string& spritePath, SDL_FRect sampleRect = SDL_FRect{});
		~SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent(SpriteRendererComponent&& other) = delete;
		SpriteRendererComponent& operator=(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent& operator=(SpriteRendererComponent&& other) = delete;

		void SetTexture(const std::shared_ptr<Texture2D>& texture);
		const std::shared_ptr<Texture2D>& GetTexture() const;

		void SetSampleRectangle(const SDL_FRect& sampleRect);
		SDL_FRect& GetSampleRectangle();

		void SetFlip(bool newValue) { m_Flip = newValue; }

		void Render() const override;
	private:
		// member functions
		bool IsSampleRectEmpty() const;

		// member variables
		TransformComponent* m_pTransformComponent;
		std::shared_ptr<Texture2D> m_pTexture;
		SDL_FRect m_SampleRectangle{};
		
		bool m_Flip{ false };
	};
}
