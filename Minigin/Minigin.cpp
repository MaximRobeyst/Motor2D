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
#include "SpriteRendererComponent.h"
#include "Time.h"
#include "FPSComponent.h"

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
		1280,
		720,
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

	auto go = new GameObject();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = new TextComponent(go.get(), "Programming 4 Assignment", font);
	//go = new GameObject();
	go->AddComponent(new TransformComponent(go, glm::vec3{10.f, 10.f, 0.f}));
	go->AddComponent(new SpriteRendererComponent(go, "logo.png"));
	go->AddComponent(new TextComponent(go, "Programming 4 Assignment", font));
	scene.Add(go);

	auto pPeperGameObject = new GameObject();

	pPeperGameObject->AddComponent(new TransformComponent(go));
	pPeperGameObject->AddComponent(new SpriteRendererComponent(go, "BurgerTime_SpriteSheet.png"));
	scene.Add(pPeperGameObject);
	
	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	//
	go = new GameObject();
	go->AddComponent(new TransformComponent(go, glm::vec3{80, 20, 0}));
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
		//auto& input = InputManager::GetInstance();

		auto t1 = std::chrono::steady_clock::now();
		float lag = 0.0f;

		// todo: this update loop could use some work.
		bool doContinue = true;
		while (doContinue)
		{
			auto t2 = std::chrono::steady_clock::now();
			float elapsedSec{ std::chrono::duration<float>(t2 - t1).count() };
			t1 = t2;
			lag += elapsedSec;

			doContinue = input.ProcessInput();

			sceneManager.Update();

			Time::GetInstance()->SetElapsed(elapsedSec);
			renderer.Render();
		}


		delete Time::GetInstance();
	}

	Cleanup();
}
