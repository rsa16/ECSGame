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

		// Modify player transform component
		if (playerComponent.is_up)
		{
			entityTransform.y -= playerComponent.movementSpeed;
		}
		else if (playerComponent.is_right)
		{
			entityTransform.x += playerComponent.movementSpeed;
		}
		else if (playerComponent.is_left)
		{
			entityTransform.x -= playerComponent.movementSpeed;
		}
		else if (playerComponent.is_down)
		{
			entityTransform.y += playerComponent.movementSpeed;
		}
	}
}
