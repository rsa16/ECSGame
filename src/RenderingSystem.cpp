#include "RenderingSystem.h"
#include "Globals.h"
#include <iostream>

extern Coordinator coordinator;

void RenderingSystem::Initialize(SDL_Renderer* pRenderer)
{
	displayTex = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	displayRect.w = DISPLAY_WIDTH;
	displayRect.h = DISPLAY_HEIGHT;
	displayRect.x = 0;
	displayRect.y = 0;

	if (displayTex == nullptr) {
		std::cout << "An error occured creating the display texture: " << SDL_GetError() << std::endl;
	}
	printf("Rendering System Initialized\n");
}

void RenderingSystem::Blit(SDL_Renderer* pRenderer, std::vector<BlitData*> thingsToBlit)
{
	// render map
	for (auto& d : thingsToBlit)
	{
		SDL_Rect emptyRect = { -1, -1, -1, -1 };
		SDL_Rect* rect;

		if (d->srcRect.x == emptyRect.x)
		{
			rect = NULL;
		}
		else {
			rect = &d->srcRect;
		}
		SDL_RenderCopy(pRenderer, d->texture, rect, &d->destRect);
		delete d;
	}
}

void RenderingSystem::Render(SDL_Renderer* pRenderer)
{
	ClearScreen(pRenderer, 0, 0, 0, 255);
	SDL_SetRenderTarget(pRenderer, displayTex);

	std::vector<BlitData*> animations;
	std::vector<BlitData*> tiles;

	// sort things to render
	for (auto& pair : thingsToBlit)
	{
		const auto& [d, code] = pair;

		switch (code)
		{
			case TILE_BLIT:
				tiles.push_back(d);
				break;

			case PLAYER_BLIT:
				animations.push_back(d);
				break;
		}
	}
	thingsToBlit.clear();

	Blit(pRenderer, tiles);
	Blit(pRenderer, animations);
}

void RenderingSystem::HandleEvents(SDL_Renderer* pRenderer, SDL_Event ev)
{
	BlitData* data = static_cast<BlitData*>(ev.user.data1);
	thingsToBlit.push_back({ data, ev.user.code });
}

void RenderingSystem::Update(SDL_Renderer* pRenderer)
{
	SDL_SetRenderTarget(pRenderer, NULL);
	SDL_RenderCopy(pRenderer, displayTex, NULL, NULL);
	SDL_RenderPresent(pRenderer);
}

void RenderingSystem::ClearScreen(SDL_Renderer* pRenderer, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
	SDL_RenderClear(pRenderer);
}

