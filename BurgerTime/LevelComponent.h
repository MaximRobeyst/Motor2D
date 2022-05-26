#pragma once
#include <Component.h>
#include <string>
#include <vector>

namespace dae
{
	class GameObject;
}

class LevelComponent : public dae::Component
{
public:
	LevelComponent() = default;
	LevelComponent(dae::GameObject* m_pGameobject, int width, int height, std::vector<std::vector<char>> level);

	void RenderGUI() override;

private:
	int m_Width{};
	int m_Height{};

	std::vector<std::vector<char>> m_Level{};
};

