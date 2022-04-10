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
#include "GameTime.h"
#include "Xbox360Controller.h"
#include "Command.h"


//#include "MovementComponent.h"
#include "SpriteRendererComponent.h"
#include "FPSComponent.h"
//#include "LifeComponent.h"
#include "LifeDisplayComponent.h"
//#include "ScoreDisplayComponent.h"
//#include "FoodComponent.h"
//#include "AchievementComponent.h"
#include "Collider.h"
#include "RigidbodyComponent.h"
//#include "PlayerComponent.h"
#include "AnimatorComponent.h"

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
	ResourceManager::GetInstance().Init("../Data/");

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

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	//Initialize();

	// tell the resource manager where he can find the game data

	//LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

		auto t1 = std::chrono::steady_clock::now();
		float lag = 0.0f;
		const float msPerUpdate = 1.f / 60.f;

		sceneManager.Start();
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
				// fixed update
				lag -= msPerUpdate;
			}

			sceneManager.Update();
			GameTime::GetInstance()->SetElapsed(elapsedSec);
			renderer.Render();

			//SteamAPI_RunCallbacks();
		}

		delete GameTime::GetInstance();
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
		for (size_t j = 0; j < s[0].size(); ++j)
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
