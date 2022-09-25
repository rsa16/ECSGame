#include "WindowSystem.h"
#include "Globals.h"

extern Coordinator coordinator;

void WindowSystem::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::stringstream buffer;
		buffer << "Could not initialize SDL properly, here is the error: " << SDL_GetError();

		throw std::runtime_error(buffer.str());
	}
	if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG) {
		std::stringstream buffer;
		buffer << IMG_GetError();

		throw std::runtime_error(buffer.str());
	}

	printf("Window System Initialized\n");
}

void WindowSystem::Cleanup()
{
	for (auto const& entity : mEntities)
	{
		auto const& windowComp = coordinator.GetComponent<Window>(entity);

		SDL_DestroyWindow(windowComp.pWindow);
		SDL_Quit();
	}
}