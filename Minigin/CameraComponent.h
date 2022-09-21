#pragma once
#include "Component.h"

namespace dae
{
	class TransformComponent;
	class CameraComponent : public Component
	{
	public:
		CameraComponent();
		CameraComponent(GameObject* pGameobject);
		
		void Start() override;
		void Update() override;
		void Render() const override;

		void RenderGUI() override;

		void SetTarget(TransformComponent* target);
		glm::vec3 WindowToGameWorld(glm::vec2 point) const;

		static CameraComponent* GetMainCamera();

	private:
		glm::vec2 Track(const glm::vec2& position);

		static CameraComponent* m_MainCamera;

		TransformComponent* m_pTransformComponent;

		TransformComponent* m_pTarget;

		float m_Speed{ 5.0f };
		glm::vec3 m_TargetPosition;

		float m_Width;
		float m_Height;

	};
}

