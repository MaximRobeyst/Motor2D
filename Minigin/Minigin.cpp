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

#include "Observer.h"
#include "Subject.h"
#include <box2d.h>
#include "ServiceLocator.h"
#include "GameStateManager.h"

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
	ServiceLocator::Cleanup();

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
			GameTime::GetInstance()->SetElapsed(elapsedSec);

			//doContinue = input.ProcessInput();
			doContinue = input.ProcessInput();
			GameStateManager::GetInstance().Update();

			while (lag >= msPerUpdate)
			{
				// fixed update
				lag -= msPerUpdate;
			}

			sceneManager.Update();
			renderer.Render();

			//SteamAPI_RunCallbacks();
		}

		GameStateManager::GetInstance().SwitchGameState(nullptr);
		GameStateManager::GetInstance().Update();
		InputManager::GetInstance().ClearInputs();
		SceneManager::GetInstance().Cleanup();
		delete GameTime::GetInstance();
	}

	Cleanup();
}
