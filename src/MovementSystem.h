#pragma once
#include "System.h"
#include "Coordinator.h"

// Components
#include "Transform.h"
#include "Player.h"

class MovementSystem :
    public System
{
public:
    void Initialize();
    void Update();
};

