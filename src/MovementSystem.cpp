#include "MovementSystem.h"

extern Coordinator coordinator;

void MovementSystem::Initialize()
{
	printf("Movement System Initialized\n");
}

void MovementSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto& entityTransform = coordinator.GetComponent<Transform>(entity);
		auto const& playerComponent = coordinator.GetComponent<Player>(entity);

		AnimationData* animData = new AnimationData();
		SDL_Event animEvent;
		animData->entity = entity;

		switch (playerComponent.direction) {
		case Direction::UP:
			if (!playerComponent.idle) {
				entityTransform.y -= playerComponent.movementSpeed;
				animData->animationState = "walk_up";
				animEvent.user.code = START_ANIMATION;
			}
			else {
				animData->animationState = "idle_up";
				animEvent.user.code = END_ANIMATION;
			}
			break;
		case Direction::DOWN:
			if (!playerComponent.idle) {
				entityTransform.y += playerComponent.movementSpeed;
				animData->animationState = "walk_down";
				animEvent.user.code = START_ANIMATION;
			}
			else {
				animData->animationState = "idle_down";
				animEvent.user.code = END_ANIMATION;
			}
			break;
		case Direction::LEFT:
			if (!playerComponent.idle) {
				entityTransform.x -= playerComponent.movementSpeed;
				animData->animationState = "walk_left";
				animEvent.user.code = START_ANIMATION;
			}
			else {
				animData->animationState = "idle_left";
				animEvent.user.code = END_ANIMATION;
			}
			break;
		case Direction::RIGHT:
			if (!playerComponent.idle) {
				entityTransform.x += playerComponent.movementSpeed;
				animData->animationState = "walk_right";
				animEvent.user.code = START_ANIMATION;
			}
			else {
				animData->animationState = "idle_right";
				animEvent.user.code = END_ANIMATION;
			}
			break;
		}

		if (animData->animationState != "")
		{
			animEvent.type = ANIMATION;
			animEvent.user.data1 = animData;
			animEvent.user.data2 = 0;

			SDL_PushEvent(&animEvent);
		}
	}
}
