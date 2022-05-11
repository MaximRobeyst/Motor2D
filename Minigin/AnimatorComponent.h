#pragma once
#include "Component.h"
#include "SpriteRendererComponent.h"
#include <map>

#include "rapidjson.h"
#include "document.h"
#include <string>
#include <SDL_rect.h>

namespace dae
{
	// the animator component holds all animations
	// the animation will hold all keyframes. keyframes tell say what part of the image you want to sample and at what time.
	// with the animator you can pause, switch and play animations

	class Animation;
	class AnimatorComponent : public Component
	{
	public:
		AnimatorComponent() = default;
		AnimatorComponent(dae::GameObject* pGameobject, const std::string& filename);
		~AnimatorComponent();

		AnimatorComponent(const AnimatorComponent& other) = delete;
		AnimatorComponent(AnimatorComponent&& other) = delete;
		AnimatorComponent& operator=(const AnimatorComponent& other) = delete;
		AnimatorComponent& operator=(AnimatorComponent&& other) = delete;

		void Start() override;
		void Update() override;

		void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
		void Deserialize(GameObject* /*pGameobject*/, rapidjson::Value& /*value*/);

		bool IsAnimationDone() const;

		void SetAnimation(int i);
		void SetAnimation(const std::string& name);
	private:
		void LoadAnimFile(const std::string& path);

		std::map<std::string, Animation*> m_pAnimations;
		Animation* m_CurrentAnimation{ nullptr };

		bool m_IsPlaying{ false };
		std::string m_Path{};

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
		Animation(float duration, float nrOfFramesPerSecond, bool looping, const rapidjson::Value& keyframes);

		void Update();

		void AddKeyFrame(const AnimationKeyframe& keyframe);

		size_t GetNrOfKeyFrames() const;
		AnimationKeyframe GetKeyFrame(int index) const;
		SDL_FRect GetCurrentKeyFrameRect() const;

		bool IsDone() const;
		bool IsLooping() const;

	private:
		AnimationKeyframe m_CurrentKeyframe;

		std::vector<AnimationKeyframe> m_KeyFrames{};
		float m_Duration;
		float m_NrOfFramePerSecond;
		bool m_Looping;

		float m_AnimTime{ 0.0f };
		int m_CurrentKeyIndex{ 0 };
	};
}



