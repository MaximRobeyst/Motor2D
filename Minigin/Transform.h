#pragma once
#include "Component.h"

#pragma warning(push, 0)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "Factory.h"
#include <box2d.h>

namespace dae
{
	class RigidbodyComponent;
	class TransformComponent final : public Component
	{
	public:
		struct Transform
		{
			glm::vec3 position{};
			float rotation{};
			glm::vec2 scale{};
		};
		TransformComponent();
		TransformComponent(GameObject* pGameobject, glm::vec3 position = glm::vec3{}, glm::vec2 scale = glm::vec2{ 1 });
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		void SetParent(dae::GameObject* pParent, bool keepPos = true);

		// Position
		const glm::vec3 GetPosition() const;
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec3& position);
		void SetPosition(const b2Vec2& position);

		void Move(const glm::vec3& moveVector);

		glm::vec2 GetForward() const;
		glm::vec2 GetRight() const;

		//Scale
		const glm::vec2 GetScale() const;
		void SetScale(float x, float y);
		void SetScale(const glm::vec2& scale);

		const float GetRotation() const;
		void SetRotation(float rot);

		void SetDirty();
		bool IsDirty() const;

		void Start() override;
		void Update() override;
		void Render() const override;

		void RenderGUI() override;

		void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
		void Deserialize(GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

		// Returns world transform
		Transform& GetTransform();
		Transform& GetWorldTransform();
		Transform& GetLocalTransform();
		const Transform& GetTransformConst() const;
		const Transform& GetWorldTransformConst() const;
		const Transform& GetLocalTransformConst() const;

	private:
		bool m_Dirty{true};

		Transform m_WorldTransform;
		Transform m_LocalTransform;
		TransformComponent* m_pParentComponent;
		RigidbodyComponent* m_pRigidbodyComponent;
		

	};
}
