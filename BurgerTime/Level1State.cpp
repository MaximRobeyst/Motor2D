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
#include "MenuCommands.h"
#include "LevelComponent.h"
#include "PlayerComponent.h"

using namespace dae;

const dae::Creator<Command, PepperCommand> g_PepperCommandCreate{};

void SingleplayerState::OnEnter()
{

	auto& scene = dae::SceneManager::GetInstance().CreateScene("SinglePlayer");
	auto& input = InputManager::GetInstance();

	auto pPlayer = new GameObject("Player");
	pPlayer->AddComponent(new TransformComponent(pPlayer, glm::vec3{ 100.f, 100.f , 0 }));
	pPlayer->AddComponent(new SpriteRendererComponent(pPlayer, "Sprites/Player/PlayerUnarmed.png", SDL_FRect{13,0,13,24}));
	auto pPlayerComp = pPlayer->AddComponent<PlayerComponent>();
	pPlayer->AddComponent(new AnimatorComponent(pPlayer, "../Data/Animations/PlayerAnimations.json"));
	pPlayer->AddComponent(new RigidbodyComponent(pPlayer));
	pPlayer->AddComponent(new ColliderComponent(pPlayer, 13, 24));

	input.AddAxis("keyboard_horizontal", new KeyboardAxis(SDLK_d, SDLK_a, input.GetKeyboard()));
	input.AddAxis("keyboard_vertical", new KeyboardAxis(SDLK_s, SDLK_w, input.GetKeyboard()));

	pPlayerComp->SetHorizontalAxis("keyboard_horizontal");
	pPlayerComp->SetVerticalAxis("keyboard_vertical");
	
	pPlayer->SetTag("Player");
	scene.AddGameObject(pPlayer);

	scene.Start();
}

void SingleplayerState::OnExit()
{
	SceneManager::GetInstance().RemoveScene(0);
}

PepperCommand::PepperCommand(PlayerComponent* pPlayerComp)
	: m_pPlayerComponent{pPlayerComp}
{
}

void PepperCommand::Execute()
{
	m_pPlayerComponent->SpawnPepper();
}

void PepperCommand::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.Key("Name");
	writer.String(typeid(*this).name());

	writer.Key("PlayerComponent");
	writer.Int(m_pPlayerComponent->GetGameObject()->GetId());
	writer.EndObject();
}

void PepperCommand::Deserialize(rapidjson::Value& value, dae::Scene* pScene)
{
	m_pPlayerComponent = pScene->GetGameobjectFromId(value["PlayerComponent"].GetInt())->GetComponent<PlayerComponent>();
}
