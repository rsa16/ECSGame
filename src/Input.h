#pragma once

#include <SDL.h>

struct Input
{
	SDL_EventType KEYUP_EVENT = SDL_KEYUP;
	SDL_EventType KEYDOWN_EVENT = SDL_KEYDOWN;

	SDL_KeyCode UP = SDLK_w;
	SDL_KeyCode DOWN = SDLK_s;
	SDL_KeyCode RIGHT = SDLK_d;
	SDL_KeyCode LEFT = SDLK_a;

	SDL_KeyCode ESCAPE = SDLK_ESCAPE;
	
	SDL_EventType QUIT = SDL_QUIT;
};