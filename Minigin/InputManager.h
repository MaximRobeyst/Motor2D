#pragma once
#include "Singleton.h"
#include <map>

namespace dae
{
	class Xbox360Controller;
	class InputManager : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void AddController(std::shared_ptr<dae::Xbox360Controller>& controller);
		std::shared_ptr<dae::Xbox360Controller> GetController(int index);

	private:
		std::vector<std::shared_ptr<dae::Xbox360Controller>> m_pXbox360Controllers{};
	};
}
