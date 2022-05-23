#include "MainMenuState.h"

#include <SceneManager.h>
#include <Scene.h>
#include <InputManager.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <TextObject.h>

#include "glm/glm.hpp"

#include "MenuCommands.h"
#include <Renderer.h>
#include <ResourceManager.h>
#include "UIButtonComponent.h"

#include <GameStateManager.h>
#include "Level1State.h"
#include "MultiplayerState.h"
#include "VersusState.h"

#include "MenuComponent.h"
#include "MenuCommands.h"

#include <ServiceLocator.h>
#include <AudioSystem.h>
#include <SDLAudioSystem.h>
#include <LoggedAudio.h>

using namespace dae;

void MainMenuState::OnEnter()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Main_Menu");
	auto& input = InputManager::GetInstance();


	GameObject* pLogo = new GameObject("Logo");
	pLogo->AddComponent(new TransformComponent(pLogo, glm::vec3{ 480.f, 100.f, 0.f}));
	pLogo->AddComponent(new SpriteRendererComponent(pLogo, "logo.png"));
	auto pMenuComp = new MenuComponent(pLogo);
	pLogo->AddComponent(pMenuComp);
	scene.AddGameObject(pLogo);

	auto pGameobject = new GameObject("Singleplayer button");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3(480.f, 200.f, 0.f)));
	pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "logo.png"));
	auto font = ResourceManager::GetInstance().LoadFont("Early GameBoy.ttf", 17);
	pGameobject->AddComponent(new TextComponent(pGameobject, "1 Player", font, SDL_Color{ 255, 255, 255 }));
	auto pUIButtonComp = new UIButtonComponent(pGameobject);
	pGameobject->AddComponent(pUIButtonComp);
	scene.AddGameObject(pGameobject);
	pGameobject->SetParent(pLogo);
	std::function<void()> newOnClickFunction = []() {GameStateManager::GetInstance().SwitchGameState(new Level1State()); };

	pUIButtonComp->SetOnClickFunction(newOnClickFunction);

	pGameobject = new GameObject("Multiplayer button");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3(480.f, 250.f, 0.f)));
	pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "logo.png"));
	pGameobject->AddComponent(new TextComponent(pGameobject, "2 Players", font, SDL_Color{ 255, 255, 255 }));
	pUIButtonComp = new UIButtonComponent(pGameobject);
	pGameobject->AddComponent(pUIButtonComp);
	pGameobject->SetParent(pLogo);
	scene.AddGameObject(pGameobject);
	newOnClickFunction = []() {GameStateManager::GetInstance().SwitchGameState(new MultiplayerState()); };

	pUIButtonComp->SetOnClickFunction(newOnClickFunction);

	pGameobject = new GameObject("Versus button");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3(480.f, 300.f, 0.f)));
	pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "logo.png"));
	pGameobject->AddComponent(new TextComponent(pGameobject, "Versus mode", font, SDL_Color{ 255, 255, 255 }));
	pUIButtonComp = new UIButtonComponent(pGameobject);
	pGameobject->AddComponent(pUIButtonComp);
	pGameobject->SetParent(pLogo);
	scene.AddGameObject(pGameobject);
	newOnClickFunction = []() {GameStateManager::GetInstance().SwitchGameState(new VersusState()); };

	pUIButtonComp->SetOnClickFunction(newOnClickFunction);


	auto keyboard = input.GetKeyboard();
	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_SPACE, KeyState::JustUp },
		std::make_unique<SwitchMenuStateCommand>()
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_e, KeyState::JustUp },
		std::make_unique<PressButtonCommand>(pMenuComp)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_UP, KeyState::JustUp },
		std::make_unique<ChangePointerCommand>(pMenuComp, -1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_DOWN, KeyState::JustUp },
		std::make_unique<ChangePointerCommand>(pMenuComp, 1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_ESCAPE, KeyState::JustUp },
		std::make_unique<SwitchMenuStateCommand>()
	);

	scene.Start();
}

void MainMenuState::OnExit()
{
	SceneManager::GetInstance().RemoveScene(0);
}
