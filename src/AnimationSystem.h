#pragma once
#include "System.h"
#include "Coordinator.h"

#include "Animation.h"
#include "Transform.h"

#include <json.hpp>
using json = nlohmann::json;

class AnimationSystem :
    public System
{
public:
    void CalculateImage(Entity entity, std::vector<std::variant<std::string, Transform>> stateTransform);
    void Render(SDL_Renderer* pRenderer);
    void Update();
    void HandleEvents(SDL_Event ev);
    void Initialize();

private:
    std::unordered_map<Entity, std::vector<std::variant<std::string, Transform>>> activeAnimations;
    std::unordered_map<std::string, std::unordered_map<std::string, json>> animationConfigs;
};

