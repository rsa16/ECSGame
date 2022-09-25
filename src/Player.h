#pragma once
#include "Globals.h"

struct Player
{
	float movementSpeed;

	bool is_up = false;
	bool is_down = false;
	bool is_left = false;
	bool is_right = false;

	bool idle = true;
	Direction direction = RIGHT;
	Direction lastDirection;
};