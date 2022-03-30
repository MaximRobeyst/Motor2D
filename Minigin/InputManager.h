#pragma once
#include "Singleton.h"
#include "Keyboard.h"
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
		std::shared_ptr<dae::Keyboard> GetKeyboard() const;

	private:
		std::shared_ptr<dae::Keyboard> m_pKeyboard{ std::make_shared<dae::Keyboard>() };
		std::vector<std::shared_ptr<dae::Xbox360Controller>> m_pXbox360Controllers{};
	};
}
