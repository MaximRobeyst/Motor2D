#include "MainMenuState.h"

#include <SceneManager.h>
#include <Scene.h>
#include <InputManager.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <TextObject.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#pragma warning (pop)

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
#include <Xbox360Controller.h>
#include "ScoreDisplayComponent.h"

using namespace dae;


struct SwitchToLevelState : ButtonFunction
{
	void operator()() override;
};

void SwitchToLevelState::operator()()
{
	GameStateManager::GetInstance().SwitchGameState(new SingleplayerState());
}

struct SwitchToMultiplayer : ButtonFunction
{
	void operator()() override;
};

void SwitchToMultiplayer::operator()()
{
	GameStateManager::GetInstance().SwitchGameState(new MultiplayerState());
}

struct SwitchToVersusState : ButtonFunction
{
	void operator()() override;
};

void SwitchToVersusState::operator()()
{
	GameStateManager::GetInstance().SwitchGameState(new VersusState());
}

dae::Creator<ButtonFunction, SwitchToLevelState> g_SwitchToLevelState{};
dae::Creator<ButtonFunction, SwitchToMultiplayer> g_SwitchToMultiplayerState{};
dae::Creator<ButtonFunction, SwitchToVersusState> g_SwitchToVersusState{};

void MainMenuState::OnEnter()
{
	ScoreDisplayComponent::ResetScore();
	auto& scene = SceneManager::GetInstance().CreateScene("Main_Menu");
	auto& input = InputManager::GetInstance();

	auto pMenupointer = new dae::GameObject("Pointer");
	auto pTransform = new dae::TransformComponent(pMenupointer, glm::vec3{}, glm::vec2{ 0.25f });
	pMenupointer->AddComponent(pTransform);
	auto pSpriteRenderer = new dae::SpriteRendererComponent(pMenupointer, "Sprites/player.png");
	pMenupointer->AddComponent(pSpriteRenderer);
	pTransform->SetPosition(glm::vec3{ 480.f, 150.f, 0.f } - glm::vec3{ pSpriteRenderer->GetSampleRectangle().w * pTransform->GetScale().x, 0.f, 0.f });


	scene.AddGameObject(pMenupointer);

	auto font = ResourceManager::GetInstance().LoadFont("Early GameBoy.ttf", 17);
	GameObject* pLogo = new GameObject("Logo");
	pLogo->AddComponent(new TransformComponent(pLogo, glm::vec3{ 480.f, 100.f, 0.f}));
	pLogo->AddComponent(new SpriteRendererComponent(pLogo, "logo.png"));
	pLogo->AddComponent(new TextComponent(pLogo, "Squared up!", font, SDL_Color{ 255, 0, 0 }));
	auto pMenuComp = new MenuComponent(pLogo, pMenupointer);
	pLogo->AddComponent(pMenuComp);
	scene.AddGameObject(pLogo);

	auto pGameobject = new GameObject("Singleplayer button");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3(0, 50, 0.f)));
	pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "logo.png"));
	pGameobject->AddComponent(new TextComponent(pGameobject, "1 Player", font, SDL_Color{ 255, 255, 255 }));
	auto pUIButtonComp = new UIButtonComponent(pGameobject);
	pGameobject->AddComponent(pUIButtonComp);
	pGameobject->SetParent(pLogo);
	//std::function<void()> newOnClickFunction = []() {GameStateManager::GetInstance().SwitchGameState(new Level1State()); };

	pUIButtonComp->SetOnClickFunction(new SwitchToLevelState());

	pGameobject = new GameObject("Multiplayer button");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3(0, 100.f, 0.f)));
	pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "logo.png"));
	pGameobject->AddComponent(new TextComponent(pGameobject, "Serializer test", font, SDL_Color{ 255, 255, 255 }));
	pUIButtonComp = new UIButtonComponent(pGameobject);
	pGameobject->AddComponent(pUIButtonComp);
	pGameobject->SetParent(pLogo);
	//newOnClickFunction = []() {GameStateManager::GetInstance().SwitchGameState(new MultiplayerState()); };

	pUIButtonComp->SetOnClickFunction(new SwitchToMultiplayer());

	auto controller = std::make_shared<dae::Xbox360Controller>(0);
	dae::InputManager::GetInstance().AddController(controller);

	auto keyboard = input.GetKeyboard();
	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_SPACE, KeyState::JustUp },
		new SwitchMenuStateCommand()
	);

	controller->AddControllerMapping(
		ControllerButtonData{ ControllerButton::ButtonA, ButtonState::Up },
		new PressButtonCommand(pMenuComp)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_e, KeyState::JustUp },
		new PressButtonCommand(pMenuComp)
	);

	controller->AddControllerMapping(
		ControllerButtonData{ ControllerButton::DPadUp, ButtonState::Up },
		new ChangePointerCommand(pMenuComp, -1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_UP, KeyState::JustUp },
		new ChangePointerCommand(pMenuComp, -1)
	);

	controller->AddControllerMapping(
		ControllerButtonData{ ControllerButton::DPadDown, ButtonState::Up },
		new ChangePointerCommand(pMenuComp, 1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_DOWN, KeyState::JustUp },
		new ChangePointerCommand(pMenuComp, 1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_ESCAPE, KeyState::JustUp },
		new SwitchMenuStateCommand()
	);

	scene.Start();
}

void MainMenuState::OnExit()
{
	SceneManager::GetInstance().RemoveScene(0);
	dae::InputManager::GetInstance().ClearInputs();
}
