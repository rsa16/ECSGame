#pragma once

#include <map>
#include <string>
#include <SDL.h>
#include "Coordinator.h"
#include <iostream>

// Components
#include "Window.h"

// Systems
#include "WindowSystem.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "RenderingSystem.h"
#include "LevelSystem.h"
#include "AnimationSystem.h"

class Game
{
private:
	SDL_Event ev;
	
	// System pointers
	std::shared_ptr<WindowSystem> windowSystem;
	std::shared_ptr<InputSystem> inputSystem;
	std::shared_ptr<MovementSystem> movementSystem;
	std::shared_ptr<RenderingSystem> renderingSystem;
	std::shared_ptr<AnimationSystem> animationSystem;
	std::shared_ptr<LevelSystem> levelSystem;

	// SDL renderer, managing outside of ecs code since the renderer is shared by all renderable entities
	SDL_Renderer* pRenderer;

public:
	Game();

	void Initialize();
	void Render();
	void Update();
	void Cleanup();

	bool HandleEvents();
};

