#include "LevelComponent.h"
#include "imgui.h"

#include <Factory.h>
#include <GameObject.h>
#include <Scene.h>

#include "PlateComponent.h"
#include "FoodComponent.h"

#include "Event.h"

const dae::Creator<dae::Component, LevelComponent> g_LevelComponentCreator{};

LevelComponent::LevelComponent(dae::GameObject* pGameobject, int width, int height, std::vector<std::vector<char>> level, std::string nextLevel)
	: dae::Component(pGameobject)
	, m_Width{width}
	, m_Height{height}
	, m_Level{level}
	, m_NextLevel{nextLevel}
{
}

void LevelComponent::Start()
{
	SetId(m_pGameObject->GetId());
}

void LevelComponent::RenderGUI()
{
	ImVec2 size{ 20.f, 20.f };

	for (int i = 0; i < m_Height; ++i)
	{
		ImGui::BeginGroup();
		for (int j = 0; j < m_Width; ++j)
		{
			if (ImGui::Button(std::to_string(m_Level[i][j]).c_str(), ImVec2(size.x, size.y)))
			{
				++m_Level[i][j];
			}
			ImGui::SameLine();
			//ImGui::Button("REACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
		}
		ImGui::EndGroup();
	}
}

void LevelComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("name");
	writer.String(typeid(*this).name());
	writer.Key("NextLevel");
	writer.String(m_NextLevel.c_str());
	writer.EndObject();
}

void LevelComponent::Deserialize(dae::GameObject* pGameobject, rapidjson::Value& value)
{
	m_pGameObject = pGameobject;
	m_NextLevel = value["NextLevel"].GetString();
}

void LevelComponent::RemoveLevel()
{
	m_pGameObject->GetScene()->RemoveGameObject(m_pGameObject);
}

void LevelComponent::AddPlates(PlateComponent* pPlateComponent)
{
	pPlateComponent->GetSubject()->AddObserver(this);
	m_pPlates.emplace_back(pPlateComponent->GetGameObject());
}

void LevelComponent::AddIngredient(FoodComponent* pFoodcomponent)
{
	pFoodcomponent->GetSubject()->AddObserver(this);
	m_pPlates.emplace_back(pFoodcomponent->GetGameObject());
}

void LevelComponent::Notify(const dae::GameObject& /*gameObject*/, const Event& /*action*/)
{
}

std::string LevelComponent::GetNextLevel() const
{
	return m_NextLevel;
}
