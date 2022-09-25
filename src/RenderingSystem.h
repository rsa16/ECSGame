#pragma once
#include "System.h"
#include "Coordinator.h"
#include <SDL.h>

#include "Renderable.h"
#include "Transform.h"
#include "Globals.h"
#include <vector>

class RenderingSystem :
    public System
{
public:
    void Initialize(SDL_Renderer* pRenderer);
    void Update(SDL_Renderer* pRenderer);
    void Render(SDL_Renderer* pRenderer);
    void ClearScreen(SDL_Renderer* pRenderer, int r, int g, int b, int a);
    void HandleEvents(SDL_Renderer* pRenderer, SDL_Event ev);
    void Blit(SDL_Renderer* pRenderer, std::vector<BlitData*> thingsToBlit);

private:
    SDL_Texture* displayTex;
    SDL_Rect displayRect;
    std::vector<std::pair<BlitData*, int>> thingsToBlit;
};

