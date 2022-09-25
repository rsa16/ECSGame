#pragma once

#include <string>
#include <SDL.h>
#include <variant>
#include <vector>
#include <map>

using std::string;

struct Animation
{
	string activeAnimationState;
	string animationType;
	bool paused;

	float current_frame = 0;
	SDL_Texture* currentImage = NULL;
	bool just_looped = false;
	std::map<string, std::vector<std::vector<std::variant<int, SDL_Texture*>>>> frameSurfsss = {};
};
