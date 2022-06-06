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

using namespace dae;

const dae::Creator<Command, PepperCommand> g_PepperCommandCreate{};

void SingleplayerState::OnEnter()
{
	dae::Scene::Deserialize("Level1");
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
