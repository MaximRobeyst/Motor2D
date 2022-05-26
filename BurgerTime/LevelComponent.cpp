#include "LevelComponent.h"
#include "imgui.h"

LevelComponent::LevelComponent(dae::GameObject* m_pGameobject, int width, int height, std::vector<std::vector<char>> level)
	: dae::Component(m_pGameObject)
	, m_Width{width}
	, m_Height{height}
	, m_Level{level}
{
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
