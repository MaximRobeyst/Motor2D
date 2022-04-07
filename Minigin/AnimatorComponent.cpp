#include "MiniginPCH.h"
#include "AnimatorComponent.h"

dae::AnimatorComponent::AnimatorComponent(dae::GameObject* pGameobject)
	: Component(pGameobject)
{
}

void dae::AnimatorComponent::Update()
{
	if (m_IsPlaying && m_CurrentAnimation != nullptr)
	{
		
	}
}

void dae::AnimatorComponent::SetAnimation(const std::string& name)
{
	m_CurrentAnimation = m_Animations[name];
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
