#include "MainMenuState.h"

#include <SceneManager.h>
#include <Scene.h>
#include <InputManager.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <TextObject.h>

#include "glm/glm.hpp"

#include "SwitchStateCommand.h"
#include <Renderer.h>
#include <ResourceManager.h>
#include "UIButtonComponent.h"

#include <GameStateManager.h>
#include "Level1State.h"

using namespace dae;

void MainMenuState::OnEnter()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Main Menu");
	auto& input = InputManager::GetInstance();


	GameObject* pGameobject = new GameObject();
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 480.f, 100.f, 0.f}));
	pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "logo.png"));
	scene.AddGameObject(pGameobject);


	pGameobject = new GameObject();
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3(480.f, 200.f, 0.f)));
	pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "logo.png"));
	auto font = ResourceManager::GetInstance().LoadFont("Early GameBoy.ttf", 17);
	pGameobject->AddComponent(new TextComponent(pGameobject, "1 Player", font, SDL_Color{ 255, 255, 255 }));
	auto pUIButtonComp = new UIButtonComponent(pGameobject);
	pGameobject->AddComponent(pUIButtonComp);
	scene.AddGameObject(pGameobject);
	std::function<void()> newOnClickFunction = []() {GameStateManager::GetInstance().SwitchGameState(new Level1State()); };

	pUIButtonComp->SetOnClickFunction(newOnClickFunction);

	pGameobject = new GameObject();
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3(480.f, 250.f, 0.f)));
	pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "logo.png"));
	pGameobject->AddComponent(new TextComponent(pGameobject, "2 Players", font, SDL_Color{ 255, 255, 255 }));
	scene.AddGameObject(pGameobject);


	auto keyboard = input.GetKeyboard();
	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_SPACE, KeyState::JustUp },
		std::make_unique<SwitchStateCommand>()
	);

	

}

void MainMenuState::OnExit()
{
	SceneManager::GetInstance().RemoveScene(0);
}
