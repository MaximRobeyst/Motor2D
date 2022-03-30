#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"
#include "Xbox360Controller.h"
#include "Keyboard.h"

bool dae::InputManager::ProcessInput()
{
	// https://gamedev.stackexchange.com/questions/190070/how-do-i-implement-an-input-wrapper-like-unitys-in-sdl2
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		m_pKeyboard->ProcessInput(&e);
	}

	for (size_t i = 0; i < m_pXbox360Controllers.size(); i++)
	{
		m_pXbox360Controllers[i]->ProcessInput();
	}

	return true;
}

void dae::InputManager::AddController(std::shared_ptr<dae::Xbox360Controller>& controller)
{
	m_pXbox360Controllers.push_back(controller);
}

std::shared_ptr<dae::Xbox360Controller> dae::InputManager::GetController(int index)
{
	return m_pXbox360Controllers[index];
}

std::shared_ptr<dae::Keyboard> dae::InputManager::GetKeyboard() const
{
	return m_pKeyboard;
}
