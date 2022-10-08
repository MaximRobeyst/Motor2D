#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <SceneManager.h>
#include <Scene.h>
#include <ResourceManager.h>
#include <InputManager.h>

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

#include "AchievementComponent.h"
#include "LifeComponent.h"
#include "LifeDisplayComponent.h"
#include "MovementComponent.h"
#include "PlayerComponent.h"
#include "ScoreDisplayComponent.h"
#include "MrHotDogComponent.h"

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#pragma warning (pop)

#include <Minigin.h>
#include "PlayAudioCommand.h"
#include <LoggedAudio.h>

#include <GameStateManager.h>
#include "MainMenuState.h"
#include "MenuComponent.h"

using namespace dae;

void LoadGame();
void MakeLevel(Scene& pScene);

int main(int, char* [])
{
	dae::Minigin engine;
	engine.Initialize();
	LoadGame();
	engine.Run();

	return 0;
}

void LoadGame()
{
#ifdef _DEBUG
	ServiceLocator::ProvideAudio(new LoggedAudio(new SDLAudioSystem()));
#else
	ServiceLocator::ProvideAudio(new SDLAudioSystem());
#endif // _DEBUG

	Factory<Component>::GetInstance().PrintRegisteredClasses();
	Factory<Command>::GetInstance().PrintRegisteredClasses();
	GameStateManager::GetInstance().SwitchGameState(new MainMenuState());
	GameStateManager::GetInstance().Update();
}