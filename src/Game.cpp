#include "Game.h"
#include "Globals.h"

extern Coordinator coordinator;

Game::Game()
{
	coordinator.Initialize();

	// Register components
	coordinator.RegisterComponent<Window>();
	coordinator.RegisterComponent<Input>();
	coordinator.RegisterComponent<Player>();
	coordinator.RegisterComponent<Level>();
	coordinator.RegisterComponent<Transform>();
	coordinator.RegisterComponent<Renderable>();
	coordinator.RegisterComponent<Animation>();

	// Register systems and set they're signature
	windowSystem = coordinator.RegisterSystem<WindowSystem>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<Window>());
		coordinator.SetSystemSignature<WindowSystem>(signature);
	}

	inputSystem = coordinator.RegisterSystem<InputSystem>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<Input>());
		coordinator.SetSystemSignature<InputSystem>(signature);
	}

	movementSystem = coordinator.RegisterSystem<MovementSystem>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<Player>());
		signature.set(coordinator.GetComponentType<Transform>());
		coordinator.SetSystemSignature<MovementSystem>(signature);
	}
	
	levelSystem = coordinator.RegisterSystem<LevelSystem>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<Level>());
		coordinator.SetSystemSignature<LevelSystem>(signature);
	}

	renderingSystem = coordinator.RegisterSystem<RenderingSystem>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<Renderable>());
		signature.set(coordinator.GetComponentType<Transform>());
		coordinator.SetSystemSignature<RenderingSystem>(signature);
	}

	animationSystem = coordinator.RegisterSystem<AnimationSystem>();
	{
		Signature signature;
		signature.set(coordinator.GetComponentType<Animation>());
		signature.set(coordinator.GetComponentType<Transform>());
		coordinator.SetSystemSignature<AnimationSystem>(signature);
	}



	// Sdl specific stuff that needs to be used alot by all systems should be managed outside of ecs, interfacing with ecs instead
	SDL_Window* pWindow = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	// Create Window
	Entity window = coordinator.CreateEntity();
	coordinator.AddComponent(window,
		Window{
			.pWindow = pWindow
		});

	// Create player entity and add necessary components
	Entity player = coordinator.CreateEntity();
	coordinator.AddComponent(player,
		Transform{
			.x = 20,
			.y = 20,
			.rotation = 0
		});

	coordinator.AddComponent(player, Input{});
	coordinator.AddComponent(player, Renderable{});
	coordinator.AddComponent(player,
		Player{
			.movementSpeed = 1.25
		});

	coordinator.AddComponent(player,
		Animation{
			.activeAnimationState = "walk_up",
			.animationType = "cat",
			.paused = false
		});
}

void Game::Initialize()
{
	windowSystem->Initialize();
	inputSystem->Initialize();
	levelSystem->Initialize(pRenderer);
	animationSystem->Initialize();
	renderingSystem->Initialize(pRenderer);
	movementSystem->Initialize();
}

void Game::Render()
{
	animationSystem->Render(pRenderer);
	renderingSystem->Render(pRenderer);
}

void Game::Update()
{
	// removing the map is simple, just dont call the level system's update
	// ecs makes debugging easier
	levelSystem->Update();
	movementSystem->Update();
	animationSystem->Update();
	renderingSystem->Update(pRenderer);
}

void Game::Cleanup()
{
	windowSystem->Cleanup();
}

bool Game::HandleEvents()
{
	while (SDL_PollEvent(&ev) != 0)
	{
		switch (ev.type) {
			case SDL_QUIT:
				return false;

			case ANIMATION:
				animationSystem->HandleEvents(ev);
				break;

			case BLIT:
				renderingSystem->HandleEvents(pRenderer, ev);
				break;

			case SDL_KEYDOWN:
				if (ev.key.keysym.sym == SDLK_ESCAPE)
				{
					return false;
				}
				inputSystem->HandleInput(ev);
				break;

			case SDL_KEYUP:
				inputSystem->HandleInput(ev);
				break;
		}
	}
	return true;
}
