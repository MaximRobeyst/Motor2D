#pragma once
#include "Component.h"
#include "SpriteRendererComponent.h"
#include <map>

namespace dae
{
	// the animator component holds all animations
	// the animation will hold all keyframes. keyframes tell say what part of the image you want to sample and at what time.
	// with the animator you can pause, switch and play animations

	class Animation;
	class AnimatorComponent : public Component
	{
	public:
		AnimatorComponent(dae::GameObject* pGameobject);
		~AnimatorComponent();

		void Update() override;

		void SetAnimation(const std::string& name);
	private:
		std::map<std::string, Animation*> m_pAnimations;
		Animation* m_CurrentAnimation{ nullptr };

		bool m_IsPlaying{ false };

		SpriteRendererComponent* m_pSpriteRendererComponent;
	};

	class Animation
	{
		struct AnimationKeyframe
		{
			float x;
			float y;
			float width;
			float height;
		};

	public:
		Animation(float duration, float nrOfFramesPerSecond);

		void Update();

		void AddKeyFrame(const AnimationKeyframe& keyframe);

		size_t GetNrOfKeyFrames() const;
		AnimationKeyframe GetKeyFrame(int index) const;
		SDL_FRect GetCurrentKeyFrameRect() const;

	private:
		AnimationKeyframe m_CurrentKeyframe;

		std::vector<AnimationKeyframe> m_KeyFrames{};
		float m_Duration;
		float m_NrOfFramePerSecond;

		float m_AnimTime{ 0.0f };
		int m_CurrentKeyIndex{ 0 };
	};
}



