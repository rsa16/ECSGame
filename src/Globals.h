#pragma once
#include <SDL.h>
#include "Types.h"
#include "Transform.h"
#include <string>

using std::string;

inline const char* GAME_TITLE = "Farming Sim?";
inline const int SCREEN_WIDTH = 900;
inline const int SCREEN_HEIGHT = 600;

inline const int DISPLAY_WIDTH = 300;
inline const int DISPLAY_HEIGHT = 200;


inline const Uint32 ANIMATION = SDL_USEREVENT;
inline const Uint32 BLIT = SDL_USEREVENT + 2;

inline const int START_ANIMATION = 0;
inline const int END_ANIMATION = 1;

// blit codes
inline const int TILE_BLIT = 0;
inline const int PLAYER_BLIT = 1;

inline const string IMAGE_PATH = "data/images/";
inline const string MAP_PATH = "data/maps/";
inline const string TILESET_PATH = "data/images/tilesets/";

typedef struct {
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect destRect;
} BlitData;

typedef struct {
	Entity entity;
	string animationState;
} AnimationData;

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};
