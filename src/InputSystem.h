#pragma once
#include "System.h"
#include "Coordinator.h"

// Components
#include "Player.h"
#include "Input.h"

class InputSystem :
    public System
{
public:
    void Initialize();
    bool HandleInput(SDL_Event ev);
};

