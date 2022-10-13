#include "MultiplayerState.h"
#include "SceneManager.h"
#include "Scene.h"

#include <GameObject.h>
#include <Transform.h>
#include <SpriteRendererComponent.h>
#include <AnimatorComponent.h>
#include "LifeComponent.h"
#include <Collider.h>
#include <RigidbodyComponent.h>
#include "PlayerComponent.h"
#include <SceneManager.h>
#include <InputManager.h>

#include <Xbox360Controller.h>
#include "ScoreDisplayComponent.h"
#include <CameraComponent.h>
#include <Renderer.h>
#include "WeaponComponent.h"
#include "ResourceManager.h"
#include "EnemySpawnerComponent.h"
#include "InteractComponent.h"
#include <Utils.h>
#include "PlayerCommands.h"
#include "Level1State.h"

using namespace dae;

void MultiplayerState::OnEnter()
{

	auto& scene = dae::SceneManager::GetInstance().CreateScene("SinglePlayer");
	auto& input = InputManager::GetInstance();

	auto pCamera = new GameObject("Camera");
	pCamera->AddComponent(new TransformComponent(pCamera, glm::vec3{}, glm::vec2{ 1,1 }));
	pCamera->AddComponent<CameraComponent>();

	scene.AddGameObject(pCamera);


	auto pPlayer = new GameObject("Player");
	pPlayer->AddComponent(new TransformComponent(pPlayer, glm::vec3{ Renderer::GetInstance().GetWindowWidth() / 2.0f, dae::Renderer::GetInstance().GetWindowHeight() / 2.0f , 0 }, glm::vec2{ 0.25f }));
	pPlayer->AddComponent(new SpriteRendererComponent(pPlayer, "Sprites/player.png", SDL_FRect{ 0,0,100.f,100.f }));
	auto pPlayerComp = pPlayer->AddComponent<PlayerComponent>();
	//pPlayer->AddComponent(new AnimatorComponent(pPlayer, "../Data/Animations/PlayerAnimations.json"));
	pPlayer->AddComponent(new RigidbodyComponent(pPlayer));
	pPlayer->AddComponent(new ColliderComponent(pPlayer, 100.f, 100.f));
	pPlayer->AddComponent(new LifeComponent(pPlayer, 1));
	auto pInteractComp = pPlayer->AddComponent<InteractComponent>();

	auto pWeapon = new GameObject("Weapon");
	pWeapon->AddComponent(new TransformComponent(pWeapon, glm::vec3{}, glm::vec2{ 5.f }));
	pWeapon->AddComponent(new SpriteRendererComponent(pWeapon, "Sprites/Arrow.png"));
	pWeapon->AddComponent(new WeaponComponent(
		Random(5, 20),
		Random(1, 10),
		Random(0.0f, static_cast<float>(M_PI / 2)),
		10,
		Random(1.0f, 5.0f)
	));

	pWeapon->SetParent(pPlayer);

	auto pAmmoDisplay = new GameObject("Ammo_Display");
	pAmmoDisplay->AddComponent(new TransformComponent(pAmmoDisplay, glm::vec3{ -2.f, -20.f, 0 }, glm::vec2{ 4.f }));
	pAmmoDisplay->AddComponent(new SpriteRendererComponent());
	auto font = ResourceManager::GetInstance().LoadFont("Early GameBoy.ttf", 17);
	pAmmoDisplay->AddComponent(new TextComponent(pAmmoDisplay, "00", font));

	pAmmoDisplay->SetParent(pPlayer);

	auto pLevelObject = new GameObject("Level");
	pLevelObject->AddComponent(new TransformComponent());

	float textureSize = 100.f;
	float scale = 0.25f;

	for (int i = 0; i < (dae::Renderer::GetInstance().GetWindowWidth() / (textureSize * scale)); ++i)
	{
		auto pWallObject = new GameObject("Wall");
		pWallObject->AddComponent(new TransformComponent(pWallObject, glm::vec3{ i * (textureSize * scale), 0.f, 0.f }, glm::vec2{ scale }));
		pWallObject->AddComponent(new SpriteRendererComponent(pWallObject, "Sprites/wall.png"));
		pWallObject->AddComponent(new RigidbodyComponent(pWallObject, b2_staticBody));
		pWallObject->AddComponent(new ColliderComponent(pWallObject, 100.f, 100.f));

		pWallObject->SetParent(pLevelObject);

		pWallObject = new GameObject("Wall");
		pWallObject->AddComponent(new TransformComponent(pWallObject, glm::vec3{ i * (textureSize * scale), dae::Renderer::GetInstance().GetWindowHeight() - (textureSize * scale), 0.f }, glm::vec2{ scale }));
		pWallObject->AddComponent(new SpriteRendererComponent(pWallObject, "Sprites/wall.png"));
		pWallObject->AddComponent(new RigidbodyComponent(pWallObject, b2_staticBody));
		pWallObject->AddComponent(new ColliderComponent(pWallObject, 100.f, 100.f));

		pWallObject->SetParent(pLevelObject);
	}

	for (int i = 0; i < (dae::Renderer::GetInstance().GetWindowHeight() / (textureSize * scale)); ++i)
	{
		auto pWallObject = new GameObject("Wall");
		pWallObject->AddComponent(new TransformComponent(pWallObject, glm::vec3{ 0.f, i * (textureSize * scale), 0.f }, glm::vec2{ scale }));
		pWallObject->AddComponent(new SpriteRendererComponent(pWallObject, "Sprites/wall.png"));
		pWallObject->AddComponent(new RigidbodyComponent(pWallObject, b2_staticBody));
		pWallObject->AddComponent(new ColliderComponent(pWallObject, 100.f, 100.f));

		pWallObject->SetParent(pLevelObject);

		pWallObject = new GameObject("Wall");
		pWallObject->AddComponent(new TransformComponent(pWallObject, glm::vec3{ dae::Renderer::GetInstance().GetWindowWidth() - (textureSize * scale),  i * (textureSize * scale), 0.f }, glm::vec2{ scale }));
		pWallObject->AddComponent(new SpriteRendererComponent(pWallObject, "Sprites/wall.png"));
		pWallObject->AddComponent(new RigidbodyComponent(pWallObject, b2_staticBody));
		pWallObject->AddComponent(new ColliderComponent(pWallObject, 100.f, 100.f));

		pWallObject->SetParent(pLevelObject);
	}

	scene.AddGameObject(pLevelObject);

	auto pScoreDisplay = new GameObject("ScoreDisplay");
	pScoreDisplay->SetTag("Score");
	pScoreDisplay->AddComponent(new TransformComponent(pScoreDisplay, glm::vec3{ dae::Renderer::GetInstance().GetWindowWidth() / 2, 0.f, 0 }, glm::vec2{ 1.f }));
	pScoreDisplay->AddComponent(new SpriteRendererComponent());
	pScoreDisplay->AddComponent(new TextComponent(pScoreDisplay, "0", font));
	pScoreDisplay->AddComponent(new ScoreDisplayComponent());

	scene.AddGameObject(pScoreDisplay);

	//pCameraComponent->SetTarget(pPlayer->GetComponent<TransformComponent>());

	input.AddAxis("keyboard_horizontal", new KeyboardAxis(SDLK_d, SDLK_a, input.GetKeyboard()));
	input.AddAxis("keyboard_vertical", new KeyboardAxis(SDLK_s, SDLK_w, input.GetKeyboard()));

	pPlayerComp->SetHorizontalAxis("keyboard_horizontal");
	pPlayerComp->SetVerticalAxis("keyboard_vertical");

	pPlayer->SetTag("Player");
	scene.AddGameObject(pPlayer);

	input.GetKeyboard()->AddKeyboardMapping(KeyboardKeyData{ SDLK_SPACE, KeyState::Down }, new AttackCommand(pPlayerComp));
	input.GetKeyboard()->AddKeyboardMapping(KeyboardKeyData{ SDLK_e,	 KeyState::Down }, new InteractCommand(pInteractComp));

	scene.Start();
}

void MultiplayerState::OnExit()
{
	dae::SceneManager::GetInstance().RemoveScene(0);
}
