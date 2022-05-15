#include "Level1State.h"

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
#include "FoodComponent.h"
#include "LifeComponent.h"
#include "LifeDisplayComponent.h"
#include "MovementComponent.h"
#include "PlayerComponent.h"
#include "ScoreDisplayComponent.h"
#include "MrHotDogComponent.h"
#include "PlateComponent.h"

#include "PlayAudioCommand.h"
#include <LoggedAudio.h>

#include <GameStateManager.h>
#include "MainMenuState.h"

#include <iostream>

using namespace dae;

void Level1State::OnEnter()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level01");
	auto& input = InputManager::GetInstance();

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
#ifdef _DEBUG
	ServiceLocator::ProvideAudio(new LoggedAudio(new SDLAudioSystem()));
#else
	ServiceLocator::ProvideAudio(new SDLAudioSystem());
#endif // _DEBUG

	//pSoundSystem->AddAudioClip("../Data/Audio/death_1.wav");

	//ServiceLocator::GetAudio()->PlaySound("../Data/Audio/burgertime_theme.wav", -1, true);

	auto go = new GameObject("Logo");
	go->AddComponent(new TransformComponent(go, glm::vec3(10.f, 5.f, 0.f)));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Score:", font, SDL_Color{ 255, 255, 255 }));
	auto pScoreDisplay = new ScoreDisplayComponent(go, 0);
	//pScoreDisplay->GetSubject()->AddObserver(pAchievmentComponent);
	go->AddComponent(pScoreDisplay);
	scene.AddGameObject(go);

	auto pPeperGameObject = new GameObject("PeterPete_Player");
	auto pPlayerTransform = new TransformComponent(pPeperGameObject, glm::vec3{ 96.f, 296.f, 0 }, glm::vec3{ 2.f });
	pPeperGameObject->AddComponent(pPlayerTransform);
	pPeperGameObject->AddComponent(new SpriteRendererComponent(pPeperGameObject, "BurgerTime_SpriteSheet.png"));
	pPeperGameObject->AddComponent(new AnimatorComponent(pPeperGameObject, "../Data/Animations/PlayerAnimations.json"));
	//pPeperGameObject->AddComponent(new MovementComponent(pPeperGameObject, 100.f));
	auto pLifeComponent = new LifeComponent{ pPeperGameObject, 3 };
	pPeperGameObject->AddComponent(pLifeComponent);
	pPeperGameObject->AddComponent(new ColliderComponent(pPeperGameObject, 15.f, 15.f, glm::vec2{ 8.0f, 8.0f }));
	pPeperGameObject->AddComponent(new RigidbodyComponent(pPeperGameObject, b2_dynamicBody, 1.f, 0.3f));
	pPeperGameObject->AddComponent(new PlayerComponent(pPeperGameObject));
	pPeperGameObject->SetTag("Player");
	scene.AddGameObject(pPeperGameObject);

	auto pHotDogGameObject = new GameObject("MrHotDog");
	pHotDogGameObject->AddComponent(new TransformComponent(pHotDogGameObject, glm::vec3(256.f, 144.f, 0), glm::vec3{ 2.f }));
	pHotDogGameObject->AddComponent(new SpriteRendererComponent(pHotDogGameObject, "BurgerTime_SpriteSheet.png"));
	pHotDogGameObject->AddComponent(new AnimatorComponent(pHotDogGameObject, "../Data/Animations/HotdogAnimations.json"));
	pHotDogGameObject->AddComponent(new ColliderComponent(pHotDogGameObject, 15.f, 15.f, glm::vec2{ 8.f, 8.5f }));
	pHotDogGameObject->AddComponent(new RigidbodyComponent(pHotDogGameObject));
	auto pEnemyComponent = new EnemyComponent(pHotDogGameObject, pPlayerTransform);
	pHotDogGameObject->AddComponent(pEnemyComponent);
	pEnemyComponent->GetSubject()->AddObserver(pScoreDisplay);
	pHotDogGameObject->SetTag("Enemy");
	scene.AddGameObject(pHotDogGameObject);

	auto pEggGameObject = new GameObject("MrEgg");
	pEggGameObject->AddComponent(new TransformComponent(pEggGameObject, glm::vec3(288.f, 144.f, 0), glm::vec3{ 2.f }));
	pEggGameObject->AddComponent(new SpriteRendererComponent(pEggGameObject, "BurgerTime_SpriteSheet.png"));
	pEggGameObject->AddComponent(new AnimatorComponent(pEggGameObject, "../Data/Animations/EggAnimations.json"));
	pEggGameObject->AddComponent(new ColliderComponent(pEggGameObject, 15.f, 15.f, glm::vec2{ 8.f, 8.5f }));
	pEggGameObject->AddComponent(new RigidbodyComponent(pEggGameObject));
	pEnemyComponent = new EnemyComponent(pEggGameObject, pPlayerTransform);
	pEggGameObject->AddComponent(pEnemyComponent);
	pEnemyComponent->GetSubject()->AddObserver(pScoreDisplay);
	pEggGameObject->SetTag("Enemy");
	scene.AddGameObject(pEggGameObject);

	auto pBurgerTop = new GameObject("BurgerTop");
	pBurgerTop->AddComponent(new TransformComponent(pBurgerTop, glm::vec3(224.f, 32.f, 0.f), glm::vec2{ 2.f }));
	pBurgerTop->AddComponent(new SpriteRendererComponent(pBurgerTop, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 48.f, 32.f, 8.f }));
	pBurgerTop->AddComponent(new ColliderComponent(pBurgerTop, 32.f, 4.f));
	pBurgerTop->AddComponent(new RigidbodyComponent(pBurgerTop, b2_dynamicBody, 1.0f, 1.0f, true));
	auto pFoodComponent = new FoodComponent(pBurgerTop);
	pBurgerTop->AddComponent(pFoodComponent);
	pBurgerTop->SetTag("Food");
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	scene.AddGameObject(pBurgerTop);

	auto pCheese = new GameObject("Cheddar");
	pCheese->AddComponent(new TransformComponent(pCheese, glm::vec3(224.f, 112.f, 0.f), glm::vec2{ 2.f }));
	pCheese->AddComponent(new SpriteRendererComponent(pCheese, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 64.f, 32.f, 8.f }));
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

	auto pPlate = new GameObject("Plate");
	pPlate->AddComponent(new TransformComponent(pPlate, glm::vec3{ 224.f, 448.f, 0.f }, glm::vec2{ 2.f }));
	pPlate->AddComponent(new SpriteRendererComponent(pPlate, "BurgerTime_SpriteSheet.png", SDL_FRect{ 112.f, 96.f, 32.f, 8.f }));
	pPlate->AddComponent(new ColliderComponent(pPlate, 32.f, 8.f, glm::vec2{ 16.f, 8.f }));
	pPlate->AddComponent(new RigidbodyComponent(pPlate, b2_dynamicBody, 1.0f, 1.0f));
	pPlate->AddComponent(new PlateComponent(pPlate));
	pPlate->SetTag("Plate");
	scene.AddGameObject(pPlate);


	go = new GameObject("Live display");
	go->AddComponent(new TransformComponent(go, glm::vec3{ 10.f, 750.f, 0.f }));
	//go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	//go->AddComponent(new TextComponent(go, "Lives: ", font, SDL_Color{ 255, 255, 0 }));
	auto pLifeDisplay = new LifeDisplayComponent(go, 3, "Lives: ");
	pLifeComponent->GetSubject()->AddObserver(pLifeDisplay);
	go->AddComponent(pLifeDisplay);
	scene.AddGameObject(go);

	auto pPeperGameObject2 = new GameObject("Second player");
	pPeperGameObject2->AddComponent(new TransformComponent(pPeperGameObject2, glm::vec3{ 900.f, 100.f, 0 }, glm::vec3{ 5, 5, 5 }));
	pPeperGameObject2->AddComponent(new SpriteRendererComponent(pPeperGameObject2, "MainCharacter.png"));
	pPeperGameObject2->AddComponent(new MovementComponent(pPeperGameObject2, 100.f));
	auto pLifeComponent2 = new LifeComponent{ pPeperGameObject2, 3 };
	pPeperGameObject2->AddComponent(pLifeComponent2);
	scene.AddGameObject(pPeperGameObject2);

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
		std::make_unique<PlayAudioCommand>()
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_UP, KeyState::JustUp },
		std::make_unique<VolumeChangeCommand>(1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_DOWN, KeyState::JustUp },
		std::make_unique<VolumeChangeCommand>(-1)
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_RIGHT, KeyState::JustUp },
		std::make_unique<StopAudioCommand>()
	);

	keyboard->AddKeyboardMapping(
		KeyboardKeyData{ SDLK_LEFT, KeyState::JustUp },
		std::make_unique<ResumeAudioCommand>()
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
	//SceneManager::GetInstance().RemoveScene(0);
}

void Level1State::MakeLevel(Scene& pScene)
{
	const int size{ 26 };
	std::string s[size]{
		"..........................",
		"..........................",
		"LLPLLPLLPLLPLLPLLPLLPLLPLL",
		"LL....LL.LL.LL....LL....LL",
		"LL....LL.LL.LL....LL....LL",
		"LL....LL.LL.LL....LL....LL",
		"LL....LL.LL.LL....LL....LL",
		"LLPLLPLL.LL.LLPLLPLLPLLPLL",
		"...LL.LL.LL.LL.LL.LL....LL",
		"...LL.LLPLLPLL.LL.LL....LL",
		"...LL.LL....LL.LL.LLPLLPLL",
		"LLPLLPLL....LL.LL.LL.LL...",
		"LL.LL.LLPPPPLLPLLPLL.LL...",
		"LL.LL.LL....LL....LL.LL...",
		"LL.LL.LL....LL....LLPLLPLL",
		"LL.LL.LL....LL....LL.LL.LL",
		"LLPLLPLLPLLPLLPLLPLL.LL.LL",
		"LL....LL....LL....LL.LL.LL",
		"LL....LL....LL....LL.LL.LL",
		"LL....LL....LL....LL.LL.LL",
		"LLPPPPLLPPPPLLPPPPLLPLLPLL",
		"..........................",
		"..........................",
		"..........................",
		"..........................",
		".BBBBBBBBBBBBBBBBBBBBBBBB."
	};

	int ladder{};
	int platform{};

	GameObject* pLevelGameobject = new GameObject("Level");
	pLevelGameobject->AddComponent(new TransformComponent(pLevelGameobject));
	pScene.AddGameObject(pLevelGameobject);

	float scale{ 2.f };
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < s[0].size(); ++j)
		{
			if (s[i][j] == '.')
				continue;

			GameObject* pGameobject = new GameObject();
			pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ j * 8.f * scale, i * 8.f * scale, 0.0f }, glm::vec3{ scale }));

			switch (s[i][j])
			{
			case 'L':
			{
				pGameobject->SetName("Ladder " + std::to_string(++ladder));
				float y{};
				if ((i + 1 < size && s[i + 1][j] == 'L' && i - 1 > 0 && s[i - 1][j] == 'L') &&
					((j + 1 < size && s[i][j + 1] == 'P') || (j - 1 > 0 && s[i][j - 1] == 'P')))
					y = 16;
				else if ((i + 1 < size && s[i + 1][j] == 'L' && i - 1 > 0 && s[i - 1][j] == 'L') && (j - 1 > 0 && s[i][j - 1] == 'P'))
					y = 16;
				else if (i + 1 < size && s[i + 1][j] == 'L' && i - 1 > 0 && s[i - 1][j] == 'L')
					y = 8;
				else if (i + 1 < size && s[i + 1][j] == '.' && i - 1 > 0 && s[i - 1][j] == 'L')
					y = 16;


				if (j + 1 < 26 && s[i][j + 1] == 'L')
					pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Level_SpriteSheet.png", SDL_FRect{ 0, y, 8, 8 }));
				else
					pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Level_SpriteSheet.png", SDL_FRect{ 8, y, 8, 8 }));
				break;
			}
			case 'P':
				pGameobject->SetName("Platform " + std::to_string(++platform));
				pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Level_SpriteSheet.png", SDL_FRect{ 16, 8, 8, 8 }));
				pGameobject->AddComponent(new ColliderComponent(pGameobject, 8.f, 4.f, glm::vec2{ 4.0f, 6.0f }));
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
}
