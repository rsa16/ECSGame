#include "AnimationSystem.h"
#include "Globals.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SDL_image.h>
#include <numeric>

namespace filesys = std::filesystem;
extern Coordinator coordinator;

// Helper function
bool hasEnding(std::string const& fullString, std::string const& ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

void AnimationSystem::Initialize()
{
	printf("Animation System Initialized\n");
}

void AnimationSystem::CalculateImage(Entity entity, std::vector<std::variant<std::string, Transform>> stateTransform)
{
	auto& animComp = coordinator.GetComponent<Animation>(entity);
	string state = std::get<string>(stateTransform[0]);
	int lastIndex = animComp.frameSurfsss[state].size() - 1;

	// calculate what the first image should be
	for (auto& [statee, frameVec] : animComp.frameSurfsss)
	{
		if (statee == state)
		{
			for (auto& frame : frameVec)
			{
				if (std::get<int>(frame[0]) > animComp.current_frame)
				{
					animComp.currentImage = std::get<SDL_Texture*>(frame[1]);
					break;
				}
			}
		}
	}
	if (std::get<int>(animComp.frameSurfsss[state][lastIndex][0]) < animComp.current_frame)
	{
		animComp.currentImage = std::get<SDL_Texture*>(animComp.frameSurfsss[state][lastIndex][1]);
	}
}

void AnimationSystem::Render(SDL_Renderer* pRenderer)
{
	for (auto const& entity : mEntities)
	{
		auto& entityAnimComp = coordinator.GetComponent<Animation>(entity);
		auto& entityTransform = coordinator.GetComponent<Transform>(entity);

		string path = "data/images/animations/" + entityAnimComp.animationType + '/';

		// if there is an active animation, set the animation systems activeanimations to match acoordingly
		if (entityAnimComp.activeAnimationState != "")
		{
			std::vector<std::variant<std::string, Transform>> stateTransform;
			stateTransform.push_back(entityAnimComp.activeAnimationState);
			stateTransform.push_back(entityTransform);

			activeAnimations.insert({ entity, stateTransform });
		}

		// if the animation type's config hasn't been written, write it
		if (animationConfigs.find(entityAnimComp.animationType) == animationConfigs.end())
		{
			// iterate through every single animation state for this type
			for (auto const& animationState : filesys::directory_iterator(path))
			{
				filesys::path p = animationState.path();
				if (filesys::exists(p))
				{
					// Two variables, one for the states path and one for the name of the state
					std::string pathString = p.string();
					std::string animationStateStr = pathString.substr(pathString.find_last_of("/") + 1, pathString.length());

					// open the state's config and write it to a stringstream buffer
					std::ifstream f(pathString + "/config.json");
					std::stringstream buffer;
					buffer << f.rdbuf();

					// Debugging, make sure all the states are being loaded and path splitting done properly
					std::cout << animationStateStr << "\n";

					json data;

					// Try and handle errors
					try
					{
						// parse the buffer using the json lib
						data = json::parse(buffer);
					}
					catch (json::exception& ex)
					{
						std::cout << ex.what() << '\n';
					}

					// store data properly, may be storing the wrong way, come back to this later and check if it causes any errors
					std::unordered_map<std::string, json> configData = animationConfigs[entityAnimComp.animationType];
					configData[animationStateStr] = data;

					animationConfigs[entityAnimComp.animationType] = configData;
					std::cout << "animation type: " << entityAnimComp.animationType << "animation state " << animationStateStr << "\n";
					std::cout << animationConfigs["cat"]["idle_up"]["loop"] << "\n";

					std::vector<SDL_Texture*> images;
					for (auto const& animationImage : filesys::directory_iterator(pathString))
					{
						filesys::path imgPath = animationImage.path();
						if (hasEnding(imgPath.string(), ".png"))
						{
							std::cout << imgPath.string() << "\n";
							images.push_back(IMG_LoadTexture(pRenderer, imgPath.string().c_str()));
						}
					}

					int totalFrames = 0;
					std::vector<std::vector<std::variant<int, SDL_Texture*>>> frameSurfss;
					for (int i = 0; i < configData[animationStateStr]["frames"].size(); i++)
					{
						totalFrames += configData[animationStateStr]["frames"][i];

						std::vector<std::variant<int, SDL_Texture*>> frameSurf;
						frameSurf.push_back(totalFrames);
						frameSurf.push_back(images[i]);

						frameSurfss.push_back(frameSurf);
					}
					entityAnimComp.frameSurfsss[animationStateStr] = frameSurfss;

					// calculate what the first image should be
					for (auto& [state, frameVec] : entityAnimComp.frameSurfsss)
					{
						for (auto& frame : frameVec)
						{
							if (std::get<int>(frame[0]) > 0)
							{
								entityAnimComp.currentImage = std::get<SDL_Texture*>(frame[1]);
								break;
							}
						}
					}
					if (std::get<int>(entityAnimComp.frameSurfsss[animationStateStr][entityAnimComp.frameSurfsss[animationStateStr].size() - 1][0]) < 0)
					{
						entityAnimComp.currentImage = std::get<SDL_Texture*>(entityAnimComp.frameSurfsss[animationStateStr][entityAnimComp.frameSurfsss[animationStateStr].size() - 1][1]);
					}
				}
			}
		}

		// if the entity isnt in the active animations map, add it (albeit the transform will be empty to signify null)
		if (activeAnimations.find(entity) == activeAnimations.end())
		{
			std::vector<std::variant<std::string, Transform>> stateTransform;
			activeAnimations.insert({ entity, stateTransform });
		}
	}
	for (auto& [entity, stateTransform] : activeAnimations)
	{
		if (!stateTransform.empty())
		{
			auto& animComp = coordinator.GetComponent<Animation>(entity);
			string state = std::get<string>(stateTransform[0]);
			Transform transform = std::get<Transform>(stateTransform[1]);
			SDL_Texture* image = animComp.currentImage;

			// get image width and height
			SDL_Point size;
			SDL_QueryTexture(image, NULL, NULL, &size.x, &size.y);

			SDL_Rect positionRect = { transform.x, transform.y, size.x, size.y };

			BlitData* data = new BlitData;
			SDL_Event blitEvent;

			data->srcRect = { -1, -1, -1, -1 };
			data->destRect = positionRect;
			data->texture = image;

			blitEvent.type = BLIT;
			blitEvent.user.code = PLAYER_BLIT;
			blitEvent.user.data1 = data;
			blitEvent.user.data2 = 0;

			SDL_PushEvent(&blitEvent);
			CalculateImage(entity, stateTransform);
		}
	}
}

void AnimationSystem::HandleEvents(SDL_Event ev)
{
	AnimationData* data = static_cast<AnimationData*>(ev.user.data1);
	Transform entityTransform = coordinator.GetComponent<Transform>(data->entity);
	Animation entityAnimComp = coordinator.GetComponent<Animation>(data->entity);

	entityAnimComp.activeAnimationState = data->animationState;
	activeAnimations[data->entity].clear();
	switch (ev.user.code)
	{
		case START_ANIMATION:
			activeAnimations[data->entity].push_back(data->animationState);
			activeAnimations[data->entity].push_back(entityTransform);
			break;

		case END_ANIMATION:
			activeAnimations[data->entity].push_back(data->animationState);
			activeAnimations[data->entity].push_back(entityTransform);
			break;

	}
}

void AnimationSystem::Update()
{
	for (auto& [entity, stateTransform] : activeAnimations)
	{
		if (!stateTransform.empty())
		{
			auto& animComp = coordinator.GetComponent<Animation>(entity);
			json anim = animationConfigs[animComp.animationType][std::get<string>(stateTransform[0])]; 

			animComp.just_looped = false;
			std::cout << "animation speed: " << anim["speed"] << "\n";
			animComp.current_frame = animComp.current_frame + 0.7;
			std::cout << "animation component current frame " << animComp.current_frame << "\n";
			if (anim["loop"])
			{
				int sum = 0;
				for (int frame : anim["frames"])
				{
					sum += frame;
				}
				while (animComp.current_frame > sum)
				{
					animComp.current_frame -= sum;
				}
			}
			CalculateImage(entity, stateTransform);
		}
	}
}