#include "Level1State.h"

#include <SceneManager.h>
#include <Scene.h>
#include <ResourceManager.h>
#include <InputManager.h>

#include "AchievementComponent.h"
#include "FoodComponent.h"
#include "LifeComponent.h"
#include "LifeDisplayComponent.h"
#include "MovementComponent.h"
#include "PlayerComponent.h"
#include "ScoreDisplayComponent.h"
#include "MrHotDogComponent.h"
#include "PlateComponent.h"
#include "EnemySpawnerComponent.h"

#include <SpriteRendererComponent.h>
#include <Transform.h>
#include <Collider.h>
#include <AnimatorComponent.h>
#include <FPSComponent.h>
#include <RigidbodyComponent.h>
#include <TextObject.h>

#include <Observer.h>
#include <Subject.h>
#include <ServiceLocator.h>
#include <AudioSystem.h>
#include <SDLAudioSystem.h>


#include "PlayAudioCommand.h"
#include <LoggedAudio.h>

#include <GameStateManager.h>

#include "MainMenuState.h"

#include <iostream>

#include "GameManagerComponent.h"
#include "LevelComponent.h"
#include "PlayerComponent.h"
#include "CameraComponent.h"
#include "WeaponComponent.h"
#include "InteractComponent.h"

#include "MenuCommands.h"
#include "PlayerCommands.h"

using namespace dae;

const dae::Creator<Command, AttackCommand> g_PepperCommandCreate{};

void SingleplayerState::OnEnter()
{

	auto& scene = dae::SceneManager::GetInstance().CreateScene("SinglePlayer");
	auto& input = InputManager::GetInstance();

	auto pCamera = new GameObject("Camera");
	pCamera->AddComponent(new TransformComponent(pCamera, glm::vec3{}, glm::vec2{ 2,2 }));
	auto pCameraComponent = pCamera->AddComponent<CameraComponent>();

	scene.AddGameObject(pCamera);

	auto pLevel = new GameObject("Level");
	pLevel->AddComponent(new TransformComponent(pLevel));
	pLevel->AddComponent(new SpriteRendererComponent(pLevel, "Scenes/Level.png"));
	scene.AddGameObject(pLevel);


	auto pPlayer = new GameObject("Player");
	pPlayer->AddComponent(new TransformComponent(pPlayer, glm::vec3{ 100.f, 100.f , 0 }));
	pPlayer->AddComponent(new SpriteRendererComponent(pPlayer, "Sprites/Player/PlayerUnarmed.png", SDL_FRect{13,0,13,24}));
	auto pPlayerComp = pPlayer->AddComponent<PlayerComponent>();
	pPlayer->AddComponent(new AnimatorComponent(pPlayer, "../Data/Animations/PlayerAnimations.json"));
	pPlayer->AddComponent(new RigidbodyComponent(pPlayer));
	pPlayer->AddComponent(new ColliderComponent(pPlayer, 13, 24));
	auto pInteractComp = pPlayer->AddComponent<InteractComponent>();

	pCameraComponent->SetTarget(pPlayer->GetComponent<TransformComponent>());

	input.AddAxis("keyboard_horizontal", new KeyboardAxis(SDLK_d, SDLK_a, input.GetKeyboard()));
	input.AddAxis("keyboard_vertical", new KeyboardAxis(SDLK_s, SDLK_w, input.GetKeyboard()));

	pPlayerComp->SetHorizontalAxis("keyboard_horizontal");
	pPlayerComp->SetVerticalAxis("keyboard_vertical");

	pPlayer->SetTag("Player");
	scene.AddGameObject(pPlayer);

	auto pLegs = new GameObject("Legs");
	pLegs->AddComponent(new TransformComponent(pLegs, glm::vec3{ 0, 0 ,0} ));
	pLegs->AddComponent(new SpriteRendererComponent(pLegs, "Sprites/Player/PlayerLegs.png", SDL_FRect{ 0,0,12,14 }));

	pLegs->SetParent(pPlayer);

	auto pWeapon = new GameObject("Weapon_M16");
	pWeapon->AddComponent(new TransformComponent(pWeapon));
	pWeapon->AddComponent(new SpriteRendererComponent(pWeapon, "Sprites/Pickups/pickupM16.png"));
	pWeapon->AddComponent(new RigidbodyComponent(pWeapon,b2_dynamicBody, 1.0f, 1.0f, true));
	pWeapon->AddComponent(new ColliderComponent(pWeapon));
	pWeapon->AddComponent(new WeaponComponent());
	scene.AddGameObject(pWeapon);

	input.GetKeyboard()->AddKeyboardMapping(KeyboardKeyData{ SDLK_SPACE, KeyState::Down }, new AttackCommand(pPlayerComp));
	input.GetKeyboard()->AddKeyboardMapping(KeyboardKeyData{ SDLK_e,	 KeyState::Down }, new InteractCommand(pInteractComp));

	scene.Start();
}

void SingleplayerState::OnExit()
{
	SceneManager::GetInstance().RemoveScene(0);
}

AttackCommand::AttackCommand(PlayerComponent* pPlayerComp)
	: m_pPlayerComponent{pPlayerComp}
{
}

void AttackCommand::Execute()
{
	m_pPlayerComponent->Attack();
}

void AttackCommand::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());

	writer.Key("PlayerComponent");
	writer.Int(m_pPlayerComponent->GetGameObject()->GetId());
	writer.EndObject();
}

void AttackCommand::Deserialize(rapidjson::Value& value, dae::Scene* pScene)
{
	m_pPlayerComponent = pScene->GetGameobjectFromId(value["PlayerComponent"].GetInt())->GetComponent<PlayerComponent>();
}
