#pragma once
#include "System.h"
#include "Window.h"
#include "Coordinator.h"
#include <sstream>
#include "SDL_Image.h"

class WindowSystem :
    public System
{
public:
    void Initialize();
    void Cleanup(); 
};

