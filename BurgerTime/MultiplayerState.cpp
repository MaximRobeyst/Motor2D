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

void MultiplayerState::OnEnter()
{
	dae::Scene::Deserialize("Multiplayer");
}

void MultiplayerState::OnExit()
{
	dae::SceneManager::GetInstance().RemoveScene(0);
}
