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
#include "FoodComponent.h"
#include "LifeComponent.h"
#include "LifeDisplayComponent.h"
#include "MovementComponent.h"
#include "PlayerComponent.h"
#include "ScoreDisplayComponent.h"
#include "MrHotDogComponent.h"
#include "PlateComponent.h"

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
	//auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	//auto& input = InputManager::GetInstance();

	GameStateManager::GetInstance().SwitchGameState(new MainMenuState());
}


void MakeLevel(Scene& pScene)
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

	float scale{ 2.f };
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < s[0].size(); ++j)
		{
			if (s[i][j] == '.')
				continue;

			GameObject* pGameobject = new GameObject;
			pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ j * 8.f * scale, i * 8.f * scale, 0.0f }, glm::vec3{ scale }));

			switch (s[i][j])
			{
			case 'L':
			{
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
				pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Level_SpriteSheet.png", SDL_FRect{16, 8, 8, 8}));
				pGameobject->AddComponent(new ColliderComponent(pGameobject, 8.f, 4.f, glm::vec2{4.0f, 6.0f}));
				pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
				break;
			case'.':

				break;
			}



			pScene.AddGameObject(pGameobject);


			//pGameobject->AddComponent(new TransformComponent(pGameobject, glm::vec3{ 13 * 16.f * scale, 0.f * 8.f * scale, 0.0f }, glm::vec3{ scale }));
			//pGameobject->AddComponent(new SpriteRendererComponent(pGameobject, "Platform.png"));
			//pGameobject->AddComponent(new ColliderComponent(pGameobject));
			//pGameobject->AddComponent(new RigidbodyComponent(pGameobject, b2_staticBody));
		}
	}

}