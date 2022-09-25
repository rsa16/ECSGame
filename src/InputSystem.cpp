#include "InputSystem.h"
#include "Globals.h"
#include <iostream>

extern Coordinator coordinator;

void InputSystem::Initialize()
{
	printf("Input System Initialized\n");
}

bool InputSystem::HandleInput(SDL_Event ev)
{
	for (auto const& entity : mEntities)
	{
		auto const& entityInputComponent = coordinator.GetComponent<Input>(entity);
		auto& playerComponent = coordinator.GetComponent<Player>(entity);

		if (ev.type == entityInputComponent.KEYDOWN_EVENT)
		{
			playerComponent.idle = false;
			playerComponent.lastDirection = playerComponent.direction;
			if (ev.key.keysym.sym == entityInputComponent.UP) {
				playerComponent.direction = UP;
			}
			if (ev.key.keysym.sym == entityInputComponent.DOWN) {
				playerComponent.direction = DOWN;
			}
			if (ev.key.keysym.sym == entityInputComponent.LEFT) {
				playerComponent.direction = LEFT;
			}
			if (ev.key.keysym.sym == entityInputComponent.RIGHT) {
				playerComponent.direction = RIGHT;
			}
		}
		else if (ev.type == entityInputComponent.KEYUP_EVENT)
		{
			playerComponent.idle = true;
		}

	}
	return true;
}