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

void Level1State::OnEnter()
{
	//dae::InputManager::GetInstance().ClearInputs();

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level01");
	auto& input = InputManager::GetInstance();
	//input.ClearInputs();

	MakeLevel(scene);

	std::cout << std::endl << std::endl;
	std::cout << "=== How To Play ====" << std::endl;
	std::cout << "Use the WASD to move around" << std::endl;
	std::cout << "Press 'Space' to play a sound" << std::endl;
	std::cout << "Press 'Up' and 'Down' to change the volume" << std::endl;
	std::cout << "Press 'Right' to stop the sounds" << std::endl;
	std::cout << "Press 'Left' to Resume the sounds" << std::endl;
	std::cout << "===================" << std::endl;

	//auto pAchievmentObject = new GameObject();
	//auto pAchievmentComponent = new AchievementComponent(pAchievmentObject);
	//pAchievmentObject->AddComponent(pAchievmentComponent);
	//scene.Add(pAchievmentObject);

	auto font = ResourceManager::GetInstance().LoadFont("Early GameBoy.ttf", 17);

	//SDLAudioSystem* pSoundSystem = new SDLAudioSystem();
	//LoggedAudio* pLoggedAudioSystem = new LoggedAudio(pSoundSystem);

	//ServiceLocator::ProvideAudio(pSoundSystem);
	//pSoundSystem->AddAudioClip("../Data/Audio/death_1.wav");

	//ServiceLocator::GetAudio()->PlaySound("../Data/Audio/burgertime_theme.wav", -1, true);

	auto go = new GameObject("Logo");
	go->AddComponent(new TransformComponent(go, glm::vec3(10.f, 5.f, 0.f)));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Score:", font, SDL_Color{ 255, 255, 255 }));
	auto pScoreDisplay = new ScoreDisplayComponent(go, 0);
	//pScoreDisplay->GetSubject()->AddObserver(pAchievmentComponent);
	go->AddComponent(pScoreDisplay);
	go->SetTag("Score");
	scene.AddGameObject(go);

	m_pPlayerObject = new GameObject("PeterPete_Player");
	auto pPlayerTransform = new TransformComponent(m_pPlayerObject, glm::vec3{ 96.f, 296.f, 0 }, glm::vec3{ 2.f });
	m_pPlayerObject->AddComponent(pPlayerTransform);
	m_pPlayerObject->AddComponent(new SpriteRendererComponent(m_pPlayerObject, "BurgerTime_SpriteSheet.png"));
	m_pPlayerObject->AddComponent(new AnimatorComponent(m_pPlayerObject, "../Data/Animations/PlayerAnimations.json"));
	//pPeperGameObject->AddComponent(new MovementComponent(pPeperGameObject, 100.f));
	auto pLifeComponent = new LifeComponent{ m_pPlayerObject, 3 };
	m_pPlayerObject->AddComponent(pLifeComponent);
	m_pPlayerObject->AddComponent(new ColliderComponent(m_pPlayerObject, 15.f, 15.f, glm::vec2{ 8.0f, 8.0f }));
	m_pPlayerObject->AddComponent(new RigidbodyComponent(m_pPlayerObject, b2_dynamicBody, 1.f, 0.3f));
	auto pPlayerComp = new PlayerComponent(m_pPlayerObject);
	m_pPlayerObject->AddComponent(pPlayerComp);
	
	InputManager::GetInstance().AddAxis("keyboard_horizontal", new KeyboardAxis(SDLK_d, SDLK_a, InputManager::GetInstance().GetKeyboard()));
	InputManager::GetInstance().AddAxis("keyboard_vertical", new KeyboardAxis(SDLK_s, SDLK_w, InputManager::GetInstance().GetKeyboard()));
	
	pPlayerComp->SetHorizontalAxis(InputManager::GetInstance().GetAxis("keyboard_horizontal"));
	pPlayerComp->SetVerticalAxis(InputManager::GetInstance().GetAxis("keyboard_vertical"));
	
	m_pPlayerObject->SetTag("Player");
	scene.AddGameObject(m_pPlayerObject);


	auto pBurgerTop = new GameObject("BurgerTop 1");
	pBurgerTop->AddComponent(new TransformComponent(pBurgerTop, glm::vec3(32.f, 112.f, 0.f), glm::vec2{ 2.f }));
	pBurgerTop->AddComponent(new SpriteRendererComponent(pBurgerTop, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 48.f, 32.f, 8.f }));
	pBurgerTop->AddComponent(new ColliderComponent(pBurgerTop, 32.f, 4.f));
	pBurgerTop->AddComponent(new RigidbodyComponent(pBurgerTop, b2_dynamicBody, 1.0f, 1.0f, true));
	auto pFoodComponent = new FoodComponent(pBurgerTop, true);
	pBurgerTop->AddComponent(pFoodComponent);
	pBurgerTop->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerTop);


	auto pCheese = new GameObject("Lettuce");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(224.f, 112.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 88.f, 32.f, 8.f }));
	pCheese->AddComponent(new ColliderComponent(pCheese, 32.f, 4.f));
	pCheese->AddComponent(new RigidbodyComponent(pCheese, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pCheese);
	pCheese->AddComponent(pFoodComponent);
	pCheese->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pCheese);

	pCheese = new GameObject("Meat_Patty");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(224.f, 192.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 72.f, 32.f, 8.f }));
	pCheese->AddComponent(new ColliderComponent(pCheese, 32.f, 4.f));
	pCheese->AddComponent(new RigidbodyComponent(pCheese, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pCheese);
	pCheese->AddComponent(pFoodComponent);
	pCheese->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pCheese);

	auto pBurgerBottom = new GameObject("Burger Bottom");
	pBurgerBottom->AddComponent(new TransformComponent(pBurgerBottom, glm::vec3{ 224.f, 320.f, 0.f }, glm::vec2{ 2.f }));
	pBurgerBottom->AddComponent(new SpriteRendererComponent(pBurgerBottom, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 56.f, 32.f, 8.f }));
	pBurgerBottom->AddComponent(new ColliderComponent(pBurgerBottom, 32.f, 4.f));
	pBurgerBottom->AddComponent(new RigidbodyComponent(pBurgerBottom, b2_dynamicBody, 1.0f, 1.0, true));
	pFoodComponent = new FoodComponent(pBurgerBottom);
	pBurgerBottom->AddComponent(pFoodComponent);
	pBurgerBottom->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerBottom);

	pBurgerTop = new GameObject("BurgerTop 2");
	pBurgerTop->AddComponent(new TransformComponent(pBurgerTop, glm::vec3(128.f, 32.f, 0.f), glm::vec2{ 2.f }));
	pBurgerTop->AddComponent(new SpriteRendererComponent(pBurgerTop, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 48.f, 32.f, 8.f }));
	pBurgerTop->AddComponent(new ColliderComponent(pBurgerTop, 32.f, 4.f));
	pBurgerTop->AddComponent(new RigidbodyComponent(pBurgerTop, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pBurgerTop, true);
	pBurgerTop->AddComponent(pFoodComponent);
	pBurgerTop->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerTop);

	pCheese = new GameObject("Lettuce 2");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(320.f, 112.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 88.f, 32.f, 8.f }));
	pCheese->AddComponent(new ColliderComponent(pCheese, 32.f, 4.f));
	pCheese->AddComponent(new RigidbodyComponent(pCheese, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pCheese);
	pCheese->AddComponent(pFoodComponent);
	pCheese->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pCheese);

	pCheese = new GameObject("Meat_Patty 2");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(320.f, 160.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 72.f, 32.f, 8.f }));
	pCheese->AddComponent(new ColliderComponent(pCheese, 32.f, 4.f));
	pCheese->AddComponent(new RigidbodyComponent(pCheese, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pCheese);
	pCheese->AddComponent(pFoodComponent);
	pCheese->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pCheese);

	pBurgerBottom = new GameObject("Burger Bottom 2");
	pBurgerBottom->AddComponent(new TransformComponent(pBurgerBottom, glm::vec3{ 320.f, 224.f, 0.f }, glm::vec2{ 2.f }));
	pBurgerBottom->AddComponent(new SpriteRendererComponent(pBurgerBottom, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 56.f, 32.f, 8.f }));
	pBurgerBottom->AddComponent(new ColliderComponent(pBurgerBottom, 32.f, 4.f));
	pBurgerBottom->AddComponent(new RigidbodyComponent(pBurgerBottom, b2_dynamicBody, 1.0f, 1.0, true));
	pFoodComponent = new FoodComponent(pBurgerBottom);
	pBurgerBottom->AddComponent(pFoodComponent);
	pBurgerBottom->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerBottom);

	pBurgerTop = new GameObject("BurgerTop 3");
	pBurgerTop->AddComponent(new TransformComponent(pBurgerTop, glm::vec3(224.f, 32.f, 0.f), glm::vec2{ 2.f }));
	pBurgerTop->AddComponent(new SpriteRendererComponent(pBurgerTop, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 48.f, 32.f, 8.f }));
	pBurgerTop->AddComponent(new ColliderComponent(pBurgerTop, 32.f, 4.f));
	pBurgerTop->AddComponent(new RigidbodyComponent(pBurgerTop, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pBurgerTop, true);
	pBurgerTop->AddComponent(pFoodComponent);
	pBurgerTop->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerTop);

	pCheese = new GameObject("Lettuce 3");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(128.f, 192.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 88.f, 32.f, 8.f }));
	pCheese->AddComponent(new ColliderComponent(pCheese, 32.f, 4.f));
	pCheese->AddComponent(new RigidbodyComponent(pCheese, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pCheese);
	pCheese->AddComponent(pFoodComponent);
	pCheese->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pCheese);

	pCheese = new GameObject("Meat_Patty 3");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(128.f, 256.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 72.f, 32.f, 8.f }));
	pCheese->AddComponent(new ColliderComponent(pCheese, 32.f, 4.f));
	pCheese->AddComponent(new RigidbodyComponent(pCheese, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pCheese);
	pCheese->AddComponent(pFoodComponent);
	pCheese->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pCheese);

	pBurgerBottom = new GameObject("Burger Bottom 3");
	pBurgerBottom->AddComponent(new TransformComponent(pBurgerBottom, glm::vec3{ 128.f, 320.f, 0.f }, glm::vec2{ 2.f }));
	pBurgerBottom->AddComponent(new SpriteRendererComponent(pBurgerBottom, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 56.f, 32.f, 8.f }));
	pBurgerBottom->AddComponent(new ColliderComponent(pBurgerBottom, 32.f, 4.f));
	pBurgerBottom->AddComponent(new RigidbodyComponent(pBurgerBottom, b2_dynamicBody, 1.0f, 1.0, true));
	pFoodComponent = new FoodComponent(pBurgerBottom);
	pBurgerBottom->AddComponent(pFoodComponent);
	pBurgerBottom->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerBottom);

	pBurgerTop = new GameObject("BurgerTop 4");
	pBurgerTop->AddComponent(new TransformComponent(pBurgerTop, glm::vec3(320.f, 32.f, 0.f), glm::vec2{ 2.f }));
	pBurgerTop->AddComponent(new SpriteRendererComponent(pBurgerTop, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 48.f, 32.f, 8.f }));
	pBurgerTop->AddComponent(new ColliderComponent(pBurgerTop, 32.f, 4.f));
	pBurgerTop->AddComponent(new RigidbodyComponent(pBurgerTop, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pBurgerTop, true);
	pBurgerTop->AddComponent(pFoodComponent);
	pBurgerTop->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerTop);

	pCheese = new GameObject("Lettuce 4");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(32.f, 176.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 88.f, 32.f, 8.f }));
	pCheese->AddComponent(new ColliderComponent(pCheese, 32.f, 4.f));
	pCheese->AddComponent(new RigidbodyComponent(pCheese, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pCheese);
	pCheese->AddComponent(pFoodComponent);
	pCheese->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pCheese);

	pCheese = new GameObject("Meat_Patty 4");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(32.f, 256.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 72.f, 32.f, 8.f }));
	pCheese->AddComponent(new ColliderComponent(pCheese, 32.f, 4.f));
	pCheese->AddComponent(new RigidbodyComponent(pCheese, b2_dynamicBody, 1.0f, 1.0f, true));
	pFoodComponent = new FoodComponent(pCheese);
	pCheese->AddComponent(pFoodComponent);
	pCheese->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pCheese);

	pBurgerBottom = new GameObject("Burger Bottom 4");
	pBurgerBottom->AddComponent(new TransformComponent(pBurgerBottom, glm::vec3{ 32.f, 320.f, 0.f }, glm::vec2{ 2.f }));
	pBurgerBottom->AddComponent(new SpriteRendererComponent(pBurgerBottom, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 56.f, 32.f, 8.f }));
	pBurgerBottom->AddComponent(new ColliderComponent(pBurgerBottom, 32.f, 4.f));
	pBurgerBottom->AddComponent(new RigidbodyComponent(pBurgerBottom, b2_dynamicBody, 1.0f, 1.0, true));
	pFoodComponent = new FoodComponent(pBurgerBottom);
	pBurgerBottom->AddComponent(pFoodComponent);
	pBurgerBottom->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerBottom);


	//pBurgerBottom = new GameObject();
	//pBurgerBottom->AddComponent(new TransformComponent(pBurgerBottom, glm::vec3{ 224.f, 256.f, 0.f }, glm::vec2{ 2.f }));
	//pBurgerBottom->AddComponent(new SpriteRendererComponent(pBurgerBottom, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 64.f, 32.f, 8.f }));
	//pBurgerBottom->AddComponent(new ColliderComponent(pBurgerBottom, 32.f, 4.f));
	//pBurgerBottom->AddComponent(new RigidbodyComponent(pBurgerBottom, b2_dynamicBody, 1.0f, 1.0f));
	//pFoodComponent = new FoodComponent(pBurgerBottom);
	//pBurgerBottom->AddComponent(pFoodComponent);
	//pBurgerBottom->SetTag("Food");
	//pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	//scene.AddGameObject(pBurgerBottom);

	GameObject* pGameManager = new GameObject("GameManager");
	pGameManager->AddComponent(new TransformComponent(pGameManager));
	m_pManagerComponent = new GameManagerComponent(pGameManager, m_pPlayerObject, 1);
	pGameManager->AddComponent(m_pManagerComponent);
	scene.AddGameObject(pGameManager);

	auto pPlate = new GameObject("Plate 1");
	pPlate->AddComponent(new TransformComponent(pPlate, glm::vec3{ 32.f, 448.f, 0.f }, glm::vec2{ 2.f }));
	pPlate->AddComponent(new SpriteRendererComponent(pPlate, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 96.f, 32.f, 8.f }));
	pPlate->AddComponent(new ColliderComponent(pPlate, 32.f, 8.f, glm::vec2{ 16.f, 8.f }));
	pPlate->AddComponent(new RigidbodyComponent(pPlate, b2_staticBody, 1.0f, 1.0f, true));
	auto pPlateComponent = new PlateComponent(pPlate);
	pPlate->AddComponent(pPlateComponent);
	pPlate->SetTag("Plate");
	scene.AddGameObject(pPlate);
	pPlateComponent->GetSubject()->AddObserver(m_pManagerComponent);

	pPlate = new GameObject("Plate 2");
	pPlate->AddComponent(new TransformComponent(pPlate, glm::vec3{ 128.f, 448.f, 0.f }, glm::vec2{ 2.f }));
	pPlate->AddComponent(new SpriteRendererComponent(pPlate, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 96.f, 32.f, 8.f }));
	pPlate->AddComponent(new ColliderComponent(pPlate, 32.f, 8.f, glm::vec2{ 16.f, 8.f }));
	pPlate->AddComponent(new RigidbodyComponent(pPlate, b2_staticBody, 1.0f, 1.0f, true));
	pPlateComponent = new PlateComponent(pPlate);
	pPlate->AddComponent(pPlateComponent);
	pPlate->SetTag("Plate");
	scene.AddGameObject(pPlate);
	pPlateComponent->GetSubject()->AddObserver(m_pManagerComponent);

	pPlate = new GameObject("Plate 3");
	pPlate->AddComponent(new TransformComponent(pPlate, glm::vec3{ 224.f, 448.f, 0.f }, glm::vec2{ 2.f }));
	pPlate->AddComponent(new SpriteRendererComponent(pPlate, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 96.f, 32.f, 8.f }));
	pPlate->AddComponent(new ColliderComponent(pPlate, 32.f, 8.f, glm::vec2{ 16.f, 8.f }));
	pPlate->AddComponent(new RigidbodyComponent(pPlate, b2_staticBody, 1.0f, 1.0f, true));
	pPlateComponent = new PlateComponent(pPlate);
	pPlate->AddComponent(pPlateComponent);
	pPlate->SetTag("Plate");
	scene.AddGameObject(pPlate);
	pPlateComponent->GetSubject()->AddObserver(m_pManagerComponent);

	pPlate = new GameObject("Plate 4");
	pPlate->AddComponent(new TransformComponent(pPlate, glm::vec3{ 320.f, 448.f, 0.f }, glm::vec2{ 2.f }));
	pPlate->AddComponent(new SpriteRendererComponent(pPlate, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 96.f, 32.f, 8.f }));
	pPlate->AddComponent(new ColliderComponent(pPlate, 32.f, 8.f, glm::vec2{ 16.f, 8.f }));
	pPlate->AddComponent(new RigidbodyComponent(pPlate, b2_staticBody, 1.0f, 1.0f, true));
	pPlateComponent = new PlateComponent(pPlate);
	pPlate->AddComponent(pPlateComponent);
	pPlate->SetTag("Plate");
	scene.AddGameObject(pPlate);
	pPlateComponent->GetSubject()->AddObserver(m_pManagerComponent);
	



	go = new GameObject("Live display");
	go->AddComponent(new TransformComponent(go, glm::vec3{ 10.f, 750.f, 0.f }));
	//go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	//go->AddComponent(new TextComponent(go, "Lives: ", font, SDL_Color{ 255, 255, 0 }));
	auto pLifeDisplay = new LifeDisplayComponent(go, 3, "Lives: ");
	pLifeComponent->GetSubject()->AddObserver(pLifeDisplay);
	go->AddComponent(pLifeDisplay);
	scene.AddGameObject(go);


	//go = new GameObject();
	//go->AddComponent(new TransformComponent(go, glm::vec3{ 1100.f, 500.f, 0.f }));
	////go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	////go->AddComponent(new TextComponent(go, "", font, SDL_Color{ 0, 255, 0 }));
	//pLifeDisplay = new LifeDisplayComponent(go, 3, " ");
	//pLifeComponent2->GetSubject()->AddObserver(pLifeDisplay);
	//go->AddComponent(pLifeDisplay);
	//scene.AddGameObject(go);

	go = new GameObject("Score_Display");
	go->AddComponent(new TransformComponent(go, glm::vec3(1100.f, 550.f, 0.f)));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Score:", font, SDL_Color{ 0, 255, 0 }));
	pScoreDisplay = new ScoreDisplayComponent(go, 3, "Score: ");
	go->AddComponent(pScoreDisplay);
	scene.AddGameObject(go);

	auto keyboard = input.GetKeyboard();
	////keyboard->AddKeyboardMapping(KeyboardKeyData{ SDLK_q, KeyState::JustUp }, std::make_unique<KillCommand>(pLifeComponent2));
	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_SPACE, KeyState::JustUp },
		new PlayAudioCommand()
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_UP, KeyState::JustUp },
		new VolumeChangeCommand(1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_DOWN, KeyState::JustUp },
		new VolumeChangeCommand(-1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_RIGHT, KeyState::JustUp },
		new StopAudioCommand()
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_LEFT, KeyState::JustUp },
		new ResumeAudioCommand()
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_ESCAPE, KeyState::JustUp },
		new SwitchMenuStateCommand()
	);

	//keyboard->AddKeyboardMapping(
	//	KeyboardKeyData{ SDLK_w, KeyState::Hold },
	//	std::make_unique<MoveCommand>(pPeperGameObject->GetComponent<RigidbodyComponent>(), glm::vec2{ 0,-1}, 10.f)
	//);
	//keyboard->AddKeyboardMapping(
	//	KeyboardKeyData{ SDLK_d, KeyState::Hold },
	//	std::make_unique<MoveCommand>(pPeperGameObject->GetComponent<RigidbodyComponent>(), glm::vec2{ 1,0}, 10.f)
	//);
	//keyboard->AddKeyboardMapping(
	//	KeyboardKeyData{ SDLK_a, KeyState::Hold },
	//	std::make_unique<MoveCommand>(pPeperGameObject->GetComponent<RigidbodyComponent>(), glm::vec2{ -1,0}, 10.f)
	//);

	//std::shared_ptr<Xbox360Controller> controller = std::make_shared<Xbox360Controller>(0);
	//controller->AddControllerMapping(ControllerButtonData{ ControllerButton::ButtonA, ButtonState::Up }, std::make_unique<KillCommand>(pLifeComponent));
	//controller->AddControllerMapping(ControllerButtonData{ ControllerButton::ButtonB, ButtonState::Up }, std::make_unique<FallCommand>(pFoodComponent));
	//controller->AddControllerMapping(
	//	ControllerButtonData{ ControllerButton::DPadDown, ButtonState::Hold },
	//	std::make_unique<MoveCommand>(pPeperGameObject2->GetComponent<RigidbodyComponent>(), glm::vec2{ 0,1}, 100.f)
	//);
	//controller->AddControllerMapping(
	//	ControllerButtonData{ ControllerButton::DPadUp, ButtonState::Hold },
	//	std::make_unique<MoveCommand>(pPeperGameObject2->GetComponent<RigidbodyComponent>(), glm::vec2{ 0,-1}, 100.f)
	//);
	//controller->AddControllerMapping(
	//	ControllerButtonData{ ControllerButton::DPadRight, ButtonState::Hold },
	//	std::make_unique<MoveCommand>(pPeperGameObject2->GetComponent<RigidbodyComponent>(), glm::vec2{ 1,0}, 100.f)
	//);
	//controller->AddControllerMapping(
	//	ControllerButtonData{ ControllerButton::DPadLeft, ButtonState::Hold },
	//	std::make_unique<MoveCommand>(pPeperGameObject->GetComponent<RigidbodyComponent>(), glm::vec2{ -1,0}, 100.f)
	//);
	//input.AddController(controller);

	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	//
	go = new GameObject("FPS_Counter");
	go->AddComponent(new TransformComponent(go, glm::vec3{ 800, 750, 0 }));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Programming 4 Assignment", font, { 255,255,0 }));
	go->AddComponent(new FPSComponent(go));

	scene.AddGameObject(go);
	scene.Start();
}

void Level1State::OnExit()
{
	SceneManager::GetInstance().RemoveScene(0);
}

void Level1State::MakeLevel(Scene& pScene)
{
	std::vector<std::vector<char>> level =
	{
		{'.','.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
		{'.','.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
		{'L','L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L'},
		{'L','L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L'},
		{'L','L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L'},
		{'L','L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L'},
		{'L','L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L'},
		{'L','L', 'P', 'L', 'L', 'P', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L'},
		{'.','.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L'},
		{'.','.', '.', 'L', 'L', '.', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L'},
		{'.','.', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L'},
		{'L','L', 'P', 'L', 'L', 'P', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.'},
		{'L','L', '.', 'L', 'L', '.', 'L', 'L', 'P', 'P', 'P', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', '.', 'L', 'L', '.', '.', '.'},
		{'L','L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.'},
		{'L','L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L'},
		{'L','L', '.', 'L', 'L', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L'},
		{'L','L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L'},
		{'L','L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L'},
		{'L','L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L'},
		{'L','L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', '.', '.', '.', 'L', 'L', '.', 'L', 'L', '.', 'L', 'L'},
		{'L','L', 'P', 'P', 'P', 'P', 'L', 'L', 'P', 'P', 'P', 'P', 'L', 'L', 'P', 'P', 'P', 'P', 'L', 'L', 'P', 'L', 'L', 'P', 'L', 'L'},
		{'.','.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
		{'.','.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
		{'.','.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
		{'.','.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
		{'.','B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', '.'}
	};


	//std::string level[size]{
	//	"..........................",
	//	"..........................",
	//	"LLPLLPLLPLLPLLPLLPLLPLLPLL",
	//	"LL....LL.LL.LL....LL....LL",
	//	"LL....LL.LL.LL....LL....LL",
	//	"LL....LL.LL.LL....LL....LL",
	//	"LL....LL.LL.LL....LL....LL",
	//	"LLPLLPLL.LL.LLPLLPLLPLLPLL",
	//	"...LL.LL.LL.LL.LL.LL....LL",
	//	"...LL.LLPLLPLL.LL.LL....LL",
	//	"...LL.LL....LL.LL.LLPLLPLL",
	//	"LLPLLPLL....LL.LL.LL.LL...",
	//	"LL.LL.LLPPPPLLPLLPLL.LL...",
	//	"LL.LL.LL....LL....LL.LL...",
	//	"LL.LL.LL....LL....LLPLLPLL",
	//	"LL.LL.LL....LL....LL.LL.LL",
	//	"LLPLLPLLPLLPLLPLLPLL.LL.LL",
	//	"LL....LL....LL....LL.LL.LL",
	//	"LL....LL....LL....LL.LL.LL",
	//	"LL....LL....LL....LL.LL.LL",
	//	"LLPPPPLLPPPPLLPPPPLLPLLPLL",
	//	"..........................",
	//	"..........................",
	//	"..........................",
	//	"..........................",
	//	".BBBBBBBBBBBBBBBBBBBBBBBB."
	//};

	int ladder{};
	int platform{};

	GameObject* pLevelGameobject = new GameObject("Level");
	pLevelGameobject->AddComponent(new TransformComponent(pLevelGameobject));
	pLevelGameobject->AddComponent(new LevelComponent(pLevelGameobject, level[0].size(), level.size(), level));
	pScene.AddGameObject(pLevelGameobject);

	GameObject* pColliderObject = new GameObject("Colliders");
	pColliderObject->AddComponent(new TransformComponent(pColliderObject));
	pColliderObject->SetParent(pLevelGameobject);
	pScene.AddGameObject(pColliderObject);

	GameObject* pEnemySpawner = new GameObject("EnemySpawner");
	pEnemySpawner->AddComponent(new TransformComponent(pEnemySpawner));
	auto pEnemySpawnerComponent = new EnemySpawnerComponent(pEnemySpawner);
	pEnemySpawner->AddComponent(pEnemySpawnerComponent);
	
	pEnemySpawnerComponent->AddSpawnPosition(glm::vec3{ 384.f, 296.f, 0.f });
	pEnemySpawnerComponent->AddSpawnPosition(glm::vec3{ 0.f, 32.f, 0.f });
	pEnemySpawnerComponent->AddSpawnPosition(glm::vec3{ 0.f, 296.f, 0.f });
	pEnemySpawnerComponent->AddSpawnPosition(glm::vec3{ 416.f, 32.f, 0.f });

	pEnemySpawner->SetParent(pLevelGameobject);
	pScene.AddGameObject(pEnemySpawner);

	float scale{ 2.f };
	int width = static_cast<int>(level.size());
	for (int i = 0; i < width; ++i)
	{
		int height = static_cast<int>(level[i].size());
		for (int j = 0; j < height; ++j)
		{
			if (level[i][j] == '.')
				continue;

			GameObject* pGameobject = new GameObject();
			pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ j * 8.f * scale, i * 8.f * scale, 0.0f }, glm::vec3{ scale }));

			switch (level[i][j])
			{
			case 'L':
			{
				pGameobject->SetName("Ladder " + std::to_string(++ladder));
				float y{};
				if ((i + 1 <  width && level[i + 1][j] == 'L' && i - 1 > 0 && level[i - 1][j] == 'L') &&
					((j + 1 < height && level[i][j + 1] == 'P') || (j - 1 > 0 && level[i][j - 1] == 'P')))
					y = 16;
				else if ((i + 1 < width&& level[i + 1][j] == 'L' && i - 1 > 0 && level[i - 1][j] == 'L') && (j - 1 > 0 && level[i][j - 1] == 'P'))
					y = 16;
				else if (i + 1 < width && level[i + 1][j] == 'L' && i - 1 > 0 && level[i - 1][j] == 'L')
					y = 8;
				else if (i + 1 < width && level[i + 1][j] == '.' && i - 1 > 0 && level[i - 1][j] == 'L')
					y = 16;


				if (j + 1 < height && level[i][j + 1] == 'L')
					pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Level_SpriteSheet.png", SDL_FRect{ 0, y, 8, 8 }));
				else
					pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Level_SpriteSheet.png", SDL_FRect{ 8, y, 8, 8 }));
				break;
			}
			case 'P':
				pGameobject->SetName("Platform " + std::to_string(++platform));
				pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Level_SpriteSheet.png", SDL_FRect{ 16, 8, 8, 8 }));
				//pGameobject->AddComponent(new ColliderComponent(pGameobject, 8.f, 4.f, glm::vec2{ 4.0f, 6.0f }));
				break;
			default:
				delete pGameobject;
				continue;
				break;
			}
			pScene.AddGameObject(pGameobject);
			pGameobject->SetParent(pLevelGameobject);

			//pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 13 * 16.f * scale, 0.f * 8.f * scale, 0.0f }, glm::vec3{ scale }));
			//pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Platform.png"));
			//pGameobject->AddComponent(new ColliderComponent(pGameobject));
			//pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
		}
	}

	GameObject* pGameobject = new GameObject("Collider 0");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 0.f, 328.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 416, 40, glm::vec2{ 208.f, 20.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 1");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 128.f, 248.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 32.f, glm::vec2{ 32.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 2");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 32.f, 248.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 32.f, glm::vec2{ 32.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 3");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 224.f, 248.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 32.f, glm::vec2{ 32.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 4");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 224.f, 184.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 32.f, glm::vec2{ 32.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 5");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 128.f, 184.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 32.f, glm::vec2{ 32.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 6");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 32.f, 160.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 48.f, glm::vec2{ 8.f, 48.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 7");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 80.f, 160.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 48.f, glm::vec2{ 8.f, 48.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 8");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 320.f, 200.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 64.f, glm::vec2{ 8.f, 64.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 9");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 368.f, 200.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 64.f, glm::vec2{ 8.f, 64.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 10");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 320.f, 152.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 32.f, glm::vec2{ 8.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 11");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 368.f, 152.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 48.f, 32.f, glm::vec2{ 24.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 12");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 344.f, 112.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 16.f, glm::vec2{ 8.f, 16.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 13");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 344.f, 16.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 48.f, glm::vec2{ 8.f, 48.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 14");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 248.f, 16.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 48.f, glm::vec2{ 8.f, 48.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 15");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 224.f, 96.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 48.f, glm::vec2{ 8.f, 48.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 16");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 272.f, 96.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 48.f, glm::vec2{ 8.f, 48.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);


	pGameobject = new GameObject("Collider 17");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 32.f, 104.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 32.f, glm::vec2{ 8.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 18");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 80.f, 104.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 32.f, glm::vec2{ 8.f, 32.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 19");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 0.f, 16.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 48.f, glm::vec2{ 64.f, 48.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 20");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 96.f, 144.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 16.f, glm::vec2{ 64.f, 16.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 21");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 128.f, 16.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 80.f, glm::vec2{ 8.f, 64.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 22");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 176.f, 16.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 16.f, 80.f, glm::vec2{ 8.f, 64.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 23");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 0.f, 0.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 416.f, 8.f, glm::vec2{ 0.f, 0.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 23");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 416.f, 0.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 416.f, glm::vec2{ 0.f, 0.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);
	pScene.AddGameObject(pGameobject);

	pGameobject = new GameObject("Collider 24");
	pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ -64.f, 0.f, 0.f }));
	pGameobject->AddComponent(new ColliderComponent(pGameobject, 64.f, 416.f, glm::vec2{ 0.f, 0.f }));
	pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
	pGameobject->SetParent(pColliderObject);

	pScene.AddGameObject(pGameobject);
}
