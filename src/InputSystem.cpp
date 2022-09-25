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

		AnimationData* animData = new AnimationData();
		SDL_Event animateEvent;

		if (ev.type == entityInputComponent.KEYDOWN_EVENT)
		{
			playerComponent.is_up = (ev.key.keysym.sym == entityInputComponent.UP);
			playerComponent.is_down = (ev.key.keysym.sym == entityInputComponent.DOWN);
			playerComponent.is_left = (ev.key.keysym.sym == entityInputComponent.LEFT);
			playerComponent.is_right = (ev.key.keysym.sym == entityInputComponent.RIGHT);

			animData->entity = entity;
			
			if (playerComponent.is_up) { animData->animationState = "walk_up"; }
			else if (playerComponent.is_down) { animData->animationState = "walk_down"; }
			else if (playerComponent.is_left) { animData->animationState = "walk_left"; }
			else if (playerComponent.is_right) { animData->animationState = "walk_right"; }

			animateEvent.user.code = START_ANIMATION;
		}
		else if (ev.type == entityInputComponent.KEYUP_EVENT)
		{
			playerComponent.is_up = (ev.key.keysym.sym == entityInputComponent.UP) ? false : playerComponent.is_up;
			playerComponent.is_down = (ev.key.keysym.sym == entityInputComponent.DOWN) ? false : playerComponent.is_down;
			playerComponent.is_left = (ev.key.keysym.sym == entityInputComponent.LEFT) ? false : playerComponent.is_left;
			playerComponent.is_right = (ev.key.keysym.sym == entityInputComponent.RIGHT) ? false : playerComponent.is_right;

			animData->entity = entity;

			if (ev.key.keysym.sym == entityInputComponent.UP) { animData->animationState = "idle_up"; }
			else if (ev.key.keysym.sym == entityInputComponent.DOWN) { animData->animationState = "idle_down"; }
			else if (ev.key.keysym.sym == entityInputComponent.LEFT) { animData->animationState = "idle_left"; }
			else if (ev.key.keysym.sym == entityInputComponent.RIGHT) { animData->animationState = "idle_right"; }

			animateEvent.user.code = END_ANIMATION;
		}

		if (animData->animationState != "")
		{
			animateEvent.type = ANIMATION;
			animateEvent.user.data1 = animData;
			animateEvent.user.data2 = 0;

			SDL_PushEvent(&animateEvent);
		}
	}
	return true;
}