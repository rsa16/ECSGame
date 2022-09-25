#include "SDL.h"
#include "Game.h"
#include <iostream>

// Global coordinator, keep this here so all the other classes can have an external symbol on it
Coordinator coordinator;

// Don't really need to worry about this stuff, all of it is just to make sure we don't max out the computer's processing speed and run at an unlimited fps. 
// Better to lock at a stable frame rate, can give option to adjust the fps lock in settings.
// We could also just enable vsync but personally i like my fps being locked to 60.
const int FPS = 60;
const int frameDelay = 1000 / FPS;

// cmon, why do you even have to read this comment to know why this is here
int main(int argc, char* argv[]) {
	Game* game = new Game();

	// Initialize
	game->Initialize();

	// once again, frame limiting stuff
	Uint32 frameStart;
	int frameTime;

	// Game loop, custom event handling function since why not
	while (game->HandleEvents()) {
		frameStart = SDL_GetTicks();

		game->Render();
		game->Update();

		// make the program sleep for the alotted time so nothing occurs within the excess frames and we can efficiently use the computers processingg power.
		// idk about u but i certainly dont want to use 100% cpu
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	return 0;
}