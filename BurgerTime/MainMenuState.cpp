#include "MainMenuState.h"

#include <SceneManager.h>
#include <Scene.h>
#include <InputManager.h>
#include "SwitchStateCommand.h"

using namespace dae;

void MainMenuState::OnEnter()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Main Menu");
	auto& input = InputManager::GetInstance();

	auto keyboard = input.GetKeyboard();
	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_SPACE, KeyState::JustUp },
		std::make_unique<SwitchStateCommand>()
	);

	

}

void MainMenuState::OnExit()
{
}
