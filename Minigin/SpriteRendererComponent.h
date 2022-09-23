#pragma once
#include "Component.h"
#include <SDL.h>

namespace dae
{
	class Texture2D;
	class TransformComponent;

	class SpriteRendererComponent final : public Component
	{
	public:
		SpriteRendererComponent() = default;
		explicit SpriteRendererComponent(GameObject* pGameObject, const std::string& spritePath, SDL_FRect sampleRect = SDL_FRect{});
		~SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent(SpriteRendererComponent&& other) = delete;
		SpriteRendererComponent& operator=(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent& operator=(SpriteRendererComponent&& other) = delete;

		void SetTexture(Texture2D* texture);
		Texture2D* GetTexture() const;

		void SetSampleRectangle(const SDL_FRect& sampleRect);
		SDL_FRect GetSampleRectangle();

		void SetFlip(bool newValue) { m_Flip = newValue; }

		void Start() override;
		void Render() const override;

		void RenderGUI() override;

		void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
		void Deserialize(GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	private:
		// member functions
		bool IsSampleRectEmpty() const;

		// member variables
		TransformComponent* m_pTransformComponent;
		Texture2D* m_pTexture;
		SDL_FRect m_SampleRectangle{};
		std::string m_Path;
		
		bool m_Flip{ false };
	};
}
