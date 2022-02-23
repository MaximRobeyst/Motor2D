#include "MiniginPCH.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>
#include <chrono>


int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window * window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();

	static int samples{10};

	// Excersise 2
	ImGui::SetNextWindowPos(ImVec2{ 10.f, 50.f });
	ImGui::SetWindowSize(ImVec2{ 350.f, 500.f });
	ImGui::Begin("Exercise 2");
	ImGui::InputInt("Samples", &samples);
	static int clicked = 0;
	if (ImGui::Button("Trash the cache"))
	{
		++clicked;
		ImGui::Text("Wait For it...");
		ShowGraph();
	}
	if (clicked & 1)
	{
		ImGui::Plot("", m_Config);
	}


	ImGui::End();

	static int samplesEx3{ 100 };
	// Excersise 3
	ImGui::SetNextWindowPos(ImVec2{ 400.f, 50.f });
	ImGui::SetWindowSize(ImVec2{ 350.f, 500.f });
	ImGui::Begin("Exercise 3");
	ImGui::InputInt("Samples", &samplesEx3);
	static int clickedGameObject = 0;
	if (ImGui::Button("Trash the cache With GameObject3D"))
	{
		++clickedGameObject;
		ImGui::Text("Wait for it...");
		ShowGraphGameObject();
	}
	if (clickedGameObject & 1)
	{
		ImGui::Plot("", m_ConfigGameObject);
	}

	static int clickedGameObjectAlt = 0;
	if (ImGui::Button("Trash the cache With GameObject3DAlt"))
	{
		++clickedGameObjectAlt;
		ImGui::Text("Wait for it...");
		ShowGraphGameObjectAlt();
	}
	if (clickedGameObjectAlt & 1)
	{
		ImGui::Plot("", m_ConfigGameObjectAlt);
		if (m_ConfigGameObjectComb.values.ys == nullptr && !timeMeasuresGameObject.empty())
		{
			ShowGraphGameObjectComb();
		}
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_Renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::ShowGraph()
{
	timeMeasures.clear();

	std::vector<int> arr(100000000);
	std::vector<float> stepSizes{};
	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		stepSizes.push_back(static_cast<float>(stepsize));
		auto start = std::chrono::high_resolution_clock::now();
	
		for (int i = 0; i < static_cast<int>(arr.size()); i += stepsize)
		{
			arr[i] *= 2;
		}
	
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count());
		timeMeasures.push_back(static_cast<float>(elapsedTime));
	}
	static uint32_t selection_start = 0, selection_length = 0;

	m_Config.values.count = timeMeasures.size();
	m_Config.values.ys = timeMeasures.data(); // use ys_list to draw several lines simultaneously
	m_Config.values.ys_count = timeMeasures.size();
	m_Config.values.color = ImColor(255, 127, 0);
	m_Config.scale.min = 0;
	m_Config.scale.max = *std::max_element(timeMeasures.begin(), timeMeasures.end());
	m_Config.tooltip.show = true;
	m_Config.grid_x.show = true;
	m_Config.grid_x.size = 128;
	m_Config.grid_x.subticks = 4;
	m_Config.grid_y.show = true;
	m_Config.grid_y.size = 100;
	m_Config.grid_y.subticks = 10;
	m_Config.selection.show = true;
	m_Config.selection.start = &selection_start;
	m_Config.selection.length = &selection_length;
	m_Config.frame_size = ImVec2(200, 100);

}

void dae::Renderer::ShowGraphGameObject()
{
	timeMeasuresGameObject.clear();

	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	class GameObject3D
	{
	public:
		Transform transform;
		int ID;
	};

	std::vector<GameObject3D> arr(10000000);
	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < static_cast<int>(arr.size()); i += stepsize)
		{
			arr[i].ID *= 2;
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();
		timeMeasuresGameObject.push_back(static_cast<float>(elapsedTime));
	}
	static uint32_t selection_start = 0, selection_length = 0;

	m_ConfigGameObject.values.count = timeMeasuresGameObject.size();
	m_ConfigGameObject.values.ys = timeMeasuresGameObject.data(); // use ys_list to draw several lines simultaneously
	m_ConfigGameObject.values.ys_count = timeMeasuresGameObject.size();
	m_ConfigGameObject.values.color = ImColor(0, 255, 0);
	m_ConfigGameObject.scale.min = 0;
	m_ConfigGameObject.scale.max = timeMeasuresGameObject[0];
	m_ConfigGameObject.tooltip.show = true;
	m_ConfigGameObject.grid_x.show = true;
	m_ConfigGameObject.grid_x.size = 128;
	m_ConfigGameObject.grid_x.subticks = 4;
	m_ConfigGameObject.grid_y.show = true;
	m_ConfigGameObject.grid_y.size = 100;
	m_ConfigGameObject.grid_y.subticks = 10;
	m_ConfigGameObject.selection.show = true;
	m_ConfigGameObject.selection.start = &selection_start;
	m_ConfigGameObject.selection.length = &selection_length;
	m_ConfigGameObject.frame_size = ImVec2(200, 100);


}

void dae::Renderer::ShowGraphGameObjectAlt()
{
	timeMeasuresGameObjectAlt.clear();
	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	class GameObject3DAlt
	{
	public:
		Transform* transform;
		int ID;
	};

	std::vector<GameObject3DAlt> arr(10000000);
	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < static_cast<int>(arr.size()); i += stepsize)
		{
			arr[i].ID *= 2;
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();
		timeMeasuresGameObjectAlt.push_back(static_cast<float>(elapsedTime));
	}
	static uint32_t selection_start = 0, selection_length = 0;

	m_ConfigGameObjectAlt.values.count = timeMeasuresGameObjectAlt.size();
	m_ConfigGameObjectAlt.values.ys = timeMeasuresGameObjectAlt.data(); // use ys_list to draw several lines simultaneously
	m_ConfigGameObjectAlt.values.ys_count = timeMeasuresGameObjectAlt.size();
	m_ConfigGameObjectAlt.values.color = ImColor(0, 255, 127);
	m_ConfigGameObjectAlt.scale.min = 0;
	m_ConfigGameObjectAlt.scale.max = timeMeasuresGameObjectAlt[0];
	m_ConfigGameObjectAlt.tooltip.show = true;
	m_ConfigGameObjectAlt.grid_x.show = true;
	m_ConfigGameObjectAlt.grid_x.size = 128;
	m_ConfigGameObjectAlt.grid_x.subticks = 4;
	m_ConfigGameObjectAlt.grid_y.show = true;
	m_ConfigGameObjectAlt.grid_y.size = 100;
	m_ConfigGameObjectAlt.grid_y.subticks = 10;
	m_ConfigGameObjectAlt.selection.show = true;
	m_ConfigGameObjectAlt.selection.start = &selection_start;
	m_ConfigGameObjectAlt.selection.length = &selection_length;
	m_ConfigGameObjectAlt.frame_size = ImVec2(200, 100);

}

void dae::Renderer::ShowGraphGameObjectComb()
{
	const float* measures[] = {&*timeMeasuresGameObject.begin(), &*timeMeasuresGameObjectAlt.begin()};
	static ImU32 colors[2] = { ImColor(0, 255, 0), ImColor(0, 255, 127) };
	static uint32_t selection_start = 0, selection_length = 0;

	static ImGui::PlotConfig config;

	config.values.count = timeMeasuresGameObjectAlt.size();
	config.values.ys_list = measures; // use ys_list to draw several lines simultaneously
	config.values.ys_count = 2;
	config.values.colors = colors;
	config.scale.min = 0;
	config.scale.max = timeMeasuresGameObject[0];
	config.tooltip.show = true;
	config.grid_x.show = true;
	config.grid_x.size = 128;
	config.grid_x.subticks = 4;
	config.grid_y.show = true;
	config.grid_y.size = 100;
	config.grid_y.subticks = 10;
	config.selection.show = true;
	config.selection.start = &selection_start;
	config.selection.length = &selection_length;
	config.frame_size = ImVec2(200, 100);

	ImGui::Plot("Combined: ", config);
}
