#include "MiniginPCH.h"
#include "AnimatorComponent.h"
#include "GameTime.h"
#include "GameObject.h"

#include <fstream>
#include <istreamwrapper.h>

dae::AnimatorComponent::AnimatorComponent(dae::GameObject* pGameobject, const std::string& filename)
	: Component(pGameobject)
{
	m_pSpriteRendererComponent = pGameobject->GetComponent<SpriteRendererComponent>();

	if (std::ifstream anim{ filename })
	{
		rapidjson::IStreamWrapper isw{ anim };

		rapidjson::Document animationFile;
		animationFile.ParseStream(isw);

		for (auto iter = animationFile.Begin(); iter != animationFile.End(); ++iter)
		{
			const rapidjson::Value& animation = *iter;

			const rapidjson::Value& animationName = animation["name"];
			m_pAnimations[animationName.GetString()] = new Animation(animation["duration"].GetFloat(), animation["framesPerSeconds"].GetFloat(), animation["looping"].GetBool(), animation["keyframes"]);
		}
	}

	SetAnimation("Idle");
	m_IsPlaying = true;
}

dae::AnimatorComponent::~AnimatorComponent()
{
	for (auto iter = m_pAnimations.begin(); iter != m_pAnimations.end(); ++iter)
	{
		delete iter->second;
	}
	m_pAnimations.clear();
}

void dae::AnimatorComponent::Update()
{
	if (m_IsPlaying && m_CurrentAnimation != nullptr)
	{
		m_CurrentAnimation->Update();
		m_pSpriteRendererComponent->SetSampleRectangle(m_CurrentAnimation->GetCurrentKeyFrameRect());
	}
}

bool dae::AnimatorComponent::IsAnimationDone() const
{
	return !m_CurrentAnimation->IsLooping() && m_CurrentAnimation->IsDone();
}

void dae::AnimatorComponent::SetAnimation(const std::string& name)
{
	if (m_CurrentAnimation == m_pAnimations[name]) return;

	m_CurrentAnimation = m_pAnimations[name];
}

dae::Animation::Animation(float duration, float nrOfFramesPerSecond, bool looping, const rapidjson::Value& keyframes)
	: m_Duration{duration}
	, m_NrOfFramePerSecond{nrOfFramesPerSecond}
	, m_Looping{looping}
{
	for (auto iter = keyframes.Begin(); iter != keyframes.End(); ++iter)
	{
		const rapidjson::Value& keyframe = *iter;

		AddKeyFrame(
			AnimationKeyframe{
				keyframe["x"].GetFloat(),
				keyframe["y"].GetFloat(),
				keyframe["w"].GetFloat(),
				keyframe["h"].GetFloat()
			}
		);
	}

	// Death!
	//AddKeyFrame(AnimationKeyframe{ 48.f, 16.f, 16.f, 16.f });
	//AddKeyFrame(AnimationKeyframe{ 64.f, 16.f, 16.f, 16.f });
	//AddKeyFrame(AnimationKeyframe{ 80.f, 16.f, 16.f, 16.f });
	//AddKeyFrame(AnimationKeyframe{ 96.f, 16.f, 16.f, 16.f });
	//AddKeyFrame(AnimationKeyframe{ 112.f, 16.f, 16.f, 16.f });
	//AddKeyFrame(AnimationKeyframe{ 128.f, 16.f, 16.f, 16.f });

	// WalkLeft
	//AddKeyFrame(AnimationKeyframe{ 48.f, 0.f, 16.f, 16.f });
	//AddKeyFrame(AnimationKeyframe{ 64.f, 0.f, 16.f, 16.f });
	//AddKeyFrame(AnimationKeyframe{ 80.f, 0.f, 16.f, 16.f });

	m_CurrentKeyframe = m_KeyFrames[0];
}

void dae::Animation::Update()
{
	m_AnimTime += GameTime::GetInstance()->GetElapsed();

	if (m_AnimTime >= (1.0f / m_NrOfFramePerSecond))
	{
		++m_CurrentKeyIndex;
		if (m_Looping && m_CurrentKeyIndex >= m_KeyFrames.size())
			m_CurrentKeyIndex = 0;
		else if (m_CurrentKeyIndex >= m_KeyFrames.size())
			m_CurrentKeyIndex = (int)m_KeyFrames.size() - 1;
		m_CurrentKeyframe = m_KeyFrames[m_CurrentKeyIndex];
		m_AnimTime -= (1.0f / m_NrOfFramePerSecond);
	}
}

void dae::Animation::AddKeyFrame(const AnimationKeyframe& keyframe)
{
	m_KeyFrames.push_back(keyframe);
}

size_t dae::Animation::GetNrOfKeyFrames() const
{
	return m_KeyFrames.size();
}

dae::Animation::AnimationKeyframe dae::Animation::GetKeyFrame(int index) const
{
	return m_KeyFrames[index];
}

SDL_FRect dae::Animation::GetCurrentKeyFrameRect() const
{
	SDL_FRect keyFrameRect;
	keyFrameRect.x = m_CurrentKeyframe.x;
	keyFrameRect.y = m_CurrentKeyframe.y;
	keyFrameRect.w = m_CurrentKeyframe.width;
	keyFrameRect.h = m_CurrentKeyframe.height;
	return keyFrameRect;
}

bool dae::Animation::IsDone() const
{
	return m_CurrentKeyIndex >= m_KeyFrames.size()-1;
}

bool dae::Animation::IsLooping() const
{
	return m_Looping;
}
