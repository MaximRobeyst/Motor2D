#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include <chrono>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"
#include "Time.h"
#include "Xbox360Controller.h"
#include "Command.h"


#include "MovementComponent.h"
#include "SpriteRendererComponent.h"
#include "FPSComponent.h"
#include "LifeComponent.h"
#include "LifeDisplayComponent.h"
#include "ScoreDisplayComponent.h"
#include "FoodComponent.h"
#include "AchievementComponent.h"
#include "Collider.h"
#include "RigidbodyComponent.h"
#include "PlayerComponent.h"

#include "Observer.h"
#include "Subject.h"
#include <box2d.h>
#include <steam_api.h>

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		896,
		768,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& input = InputManager::GetInstance();

	MakeLevel(scene);

	std::cout << std::endl << std::endl;
	std::cout << "=== How To Play ====" << std::endl;
	std::cout << "Use the Dpad to move around" << std::endl;
	std::cout << "Press 'A' to love a life" << std::endl;
	std::cout << "Press 'B' to Add to your score" << std::endl;
	std::cout << "===================" << std::endl;

	//auto pAchievmentObject = new GameObject();
	//auto pAchievmentComponent = new AchievementComponent(pAchievmentObject);
	//pAchievmentObject->AddComponent(pAchievmentComponent);
	//scene.Add(pAchievmentObject);

	auto go = new GameObject();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = new TextComponent(go.get(), "Programming 4 Assignment", font);
	//go = new GameObject();
	go->AddComponent(new TransformComponent(go, glm::vec3{10.f, 10.f, 0.f}));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Programming 4 Assignment", font));
	scene.Add(go);

	auto pPeperGameObject = new GameObject();

	pPeperGameObject->AddComponent(new TransformComponent(pPeperGameObject, glm::vec3{100, 100, 0}, glm::vec3{2.f}));
	pPeperGameObject->AddComponent(new SpriteRendererComponent(pPeperGameObject, "MainCharacter.png"));
	//pPeperGameObject->AddComponent(new MovementComponent(pPeperGameObject, 100.f));
	auto pLifeComponent = new LifeComponent{ pPeperGameObject, 3 };
	pPeperGameObject->AddComponent(pLifeComponent);
	pPeperGameObject->AddComponent(new ColliderComponent(pPeperGameObject));
	pPeperGameObject->AddComponent(new RigidbodyComponent(pPeperGameObject, b2_dynamicBody, 1.f, 1.f));
	pPeperGameObject->AddComponent(new PlayerComponent(pPeperGameObject));
	scene.Add(pPeperGameObject);

	go = new GameObject();
	go->AddComponent(new TransformComponent(go, glm::vec3{ 10.f, 500.f, 0.f }));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Lives: ", font, SDL_Color{ 255, 255, 0 }));
	auto pLifeDisplay = new LifeDisplayComponent(go, 3, "Lives: ");
	pLifeComponent->GetSubject()->AddObserver(pLifeDisplay);
	go->AddComponent(pLifeDisplay);
	scene.Add(go);

	go = new GameObject();
	go->AddComponent(new TransformComponent(go, glm::vec3(10.f, 550.f, 0.f)));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Score:", font, SDL_Color{ 255, 255, 0 }));
	auto pScoreDisplay = new ScoreDisplayComponent(go, 0, "Score: ");
	//pScoreDisplay->GetSubject()->AddObserver(pAchievmentComponent);
	go->AddComponent(pScoreDisplay);
	scene.Add(go);

	auto pBurgerBun = new GameObject();
	pBurgerBun->AddComponent(new TransformComponent(pBurgerBun, glm::vec3(1600.f, 16.0f, 0), glm::vec3(2.f)));
	pBurgerBun->AddComponent(new SpriteRendererComponent(pBurgerBun, "BurgerBun_Top.png"));
	auto pFoodComponent = new FoodComponent(pBurgerBun);
	pFoodComponent->GetSubject()->AddObserver(pScoreDisplay);
	pBurgerBun->AddComponent(pFoodComponent);
	scene.Add(pBurgerBun);
	
	auto pPeperGameObject2 = new GameObject();
	pPeperGameObject2->AddComponent(new TransformComponent(pPeperGameObject2, glm::vec3{ 900.f, 100.f, 0 }, glm::vec3{ 5, 5, 5 }));
	pPeperGameObject2->AddComponent(new SpriteRendererComponent(pPeperGameObject2, "MainCharacter.png"));
	pPeperGameObject2->AddComponent(new MovementComponent(pPeperGameObject2, 100.f));
	auto pLifeComponent2 = new LifeComponent{ pPeperGameObject2, 3 };
	pPeperGameObject2->AddComponent(pLifeComponent2);
	scene.Add(pPeperGameObject2);

	go = new GameObject();
	go->AddComponent(new TransformComponent(go, glm::vec3{ 1100.f, 500.f, 0.f }));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Lives: ", font, SDL_Color{ 0, 255, 0 }));
	pLifeDisplay = new LifeDisplayComponent(go, 3, "Lives: ");
	pLifeComponent2->GetSubject()->AddObserver(pLifeDisplay);
	go->AddComponent(pLifeDisplay);
	scene.Add(go);

	go = new GameObject();
	go->AddComponent(new TransformComponent(go, glm::vec3(1100.f, 550.f, 0.f)));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Score:", font, SDL_Color{ 0, 255, 0 }));
	pScoreDisplay = new ScoreDisplayComponent(go, 0, "Score: ");
	go->AddComponent(pScoreDisplay);
	scene.Add(go);

	auto keyboard = input.GetKeyboard();
	keyboard->AddKeyboardMapping(KeyboardKeyData{ SDLK_q, KeyState::JustUp }, std::make_unique<KillCommand>(pLifeComponent2));
	//keyboard->AddKeyboardMapping(
	//	KeyboardKeyData{ SDLK_s, KeyState::Hold },
	//	std::make_unique<MoveCommand>(pPeperGameObject->GetComponent<RigidbodyComponent>(), glm::vec2{ 0,1}, 10.f)
	//);
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

	std::shared_ptr<Xbox360Controller> controller = std::make_shared<Xbox360Controller>(0);
	controller->AddControllerMapping(ControllerButtonData{ ControllerButton::ButtonA, ButtonState::Up }, std::make_unique<KillCommand>(pLifeComponent));
	controller->AddControllerMapping(ControllerButtonData{ ControllerButton::ButtonB, ButtonState::Up }, std::make_unique<FallCommand>(pFoodComponent));
	controller->AddControllerMapping(
		ControllerButtonData{ ControllerButton::DPadDown, ButtonState::Hold },
		std::make_unique<MoveCommand>(pPeperGameObject2->GetComponent<RigidbodyComponent>(), glm::vec2{ 0,1}, 100.f)
	);
	controller->AddControllerMapping(
		ControllerButtonData{ ControllerButton::DPadUp, ButtonState::Hold },
		std::make_unique<MoveCommand>(pPeperGameObject2->GetComponent<RigidbodyComponent>(), glm::vec2{ 0,-1}, 100.f)
	);
	controller->AddControllerMapping(
		ControllerButtonData{ ControllerButton::DPadRight, ButtonState::Hold },
		std::make_unique<MoveCommand>(pPeperGameObject2->GetComponent<RigidbodyComponent>(), glm::vec2{ 1,0}, 100.f)
	);
	controller->AddControllerMapping(
		ControllerButtonData{ ControllerButton::DPadLeft, ButtonState::Hold },
		std::make_unique<MoveCommand>(pPeperGameObject->GetComponent<RigidbodyComponent>(), glm::vec2{ -1,0}, 100.f)
	);
	input.AddController(controller);

	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	//
	go = new GameObject();
	go->AddComponent(new TransformComponent(go, glm::vec3{20, 60, 0}));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Programming 4 Assignment", font, {255,255,0}));
	go->AddComponent(new FPSComponent(go));

	scene.Add(go);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

		auto t1 = std::chrono::steady_clock::now();
		float lag = 0.0f;
		const float msPerUpdate = 1.f / 60.f;

		// todo: this update loop could use some work.
		bool doContinue = true;
		while (doContinue)
		{
			auto t2 = std::chrono::steady_clock::now();
			float elapsedSec{ std::chrono::duration<float>(t2 - t1).count() };
			t1 = t2;
			lag += elapsedSec;

			//doContinue = input.ProcessInput();
			doContinue = input.ProcessInput();

			while (lag >= msPerUpdate)
			{
				sceneManager.Update();
				lag -= msPerUpdate;
			}

			Time::GetInstance()->SetElapsed(elapsedSec);
			renderer.Render();

			SteamAPI_RunCallbacks();
		}


		delete Time::GetInstance();
	}

	Cleanup();
}

void dae::Minigin::MakeLevel(Scene& pScene) const
{
	const int size{ 17 };
	//std::string s[size]{
	//"................",
	//"................",
	//"LPPPLPLPLPLPPLPPL",
	//"L...L.L.L.L..L..L",
	//"L...L.L.L.L..L..L",
	//"L...L.L.L.L..L..L",
	//"L...L.L.L.L..L..L",
	//"LPLPL.L.LPLPPLPPL",
	//"..L...L.L.L..L..L",
	//"..L....L.L..L..L",
	//"..L....L.L..L..L",
	//"PPPPPPPLPLPPLPPL",
	//"................",
	//"................",
	//"................",
	//"................",
	//"................",
	//};

	std::string s[size]
	{
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"PPPPPPPPPPPPPPPP",
		"................",
		"................",
		"................",
	};

	float scale{ 3.f };
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < s[0].size(); ++j)
		{
			if (s[i][j] == '.')
				continue;

			GameObject* pGameobject = new GameObject;
			pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ j * 16.f * scale, i * 8.f * scale, 0.0f }, glm::vec3{ scale}));

			switch (s[i][j])
			{
			case 'L':
				if(s[i - 1][j] == '.' && s[i + 1][j] == 'L')
					pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Ladder_Top.png"));
				else if((j != 0 && j != s[0].size()) && s[i][j - 1] == 'P' || s[i][j + 1] == 'P')
					pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Ladder_Top.png"));
				else if (s[i - 1][j] == '.')
					pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Ladder_Bottom.png"));
				else
					pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Ladder_Middle.png"));
				break;
			case 'P':
				pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Platform.png"));
			}
			pGameobject->AddComponent(new ColliderComponent(pGameobject, 32.f, 4.f));
			pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));



			pScene.Add(pGameobject);


			//pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 13 * 16.f * scale, 0.f * 8.f * scale, 0.0f }, glm::vec3{ scale }));
			//pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Platform.png"));
			//pGameobject->AddComponent(new ColliderComponent(pGameobject));
			//pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
		}
	}

}
