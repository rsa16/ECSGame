#pragma once
#include "System.h"
#include "Coordinator.h"
#include "Globals.h"

#include "Level.h"

#include "tileson.hpp"
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using tson::Tileson;
using tson::Map;
using tson::ParseStatus;
using tson::LayerType;
using tson::Rect;
using tson::Vector2f;

using std::map;
using std::string;
using std::unique_ptr;

class LevelSystem :
    public System
{
public:
    void Initialize(SDL_Renderer* pRenderer);
    void Update();

private:
    Tileson ts;
    map<string, SDL_Texture*> tilesets;
    map<string, unique_ptr<Map>> levelMaps;
    Entity levelMap;
};

