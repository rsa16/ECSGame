#include "LevelSystem.h"
#include "Animation.h"

extern Coordinator coordinator;

void LevelSystem::Initialize(SDL_Renderer* pRenderer)
{
	levelMap = coordinator.CreateEntity();

	coordinator.AddComponent(levelMap,
		Level{
			.levelName = "test",
			.levelDir = "idk",
			.tileSize = 16
		});

	// setup tilesets
	tilesets["Dirt"] = IMG_LoadTexture(pRenderer, (TILESET_PATH + "Tilled Dirt.png").c_str());
	tilesets["Grass"] = IMG_LoadTexture(pRenderer, (TILESET_PATH + "Grass.png").c_str());
	tilesets["Hills"] = IMG_LoadTexture(pRenderer, (TILESET_PATH + "Hills.png").c_str());
	tilesets["Objects"] = IMG_LoadTexture(pRenderer, (TILESET_PATH + "Basic Grass Biom things 1.png").c_str());
	tilesets["Water"] = IMG_LoadTexture(pRenderer, (TILESET_PATH + "Water.png").c_str());

	levelMaps["test"] = ts.parse(fs::path(MAP_PATH + "test" + ".json"));
	printf("Level System Initialized \n");
}

void LevelSystem::Update()
{
	auto& levelComp = coordinator.GetComponent<Level>(levelMap);

	if (levelMaps["test"]->getStatus() == ParseStatus::OK)
	{
		for (auto& layer : levelMaps["test"]->getLayers())
		{
			if (layer.getType() == LayerType::TileLayer)
			{
				for (auto& [pos, tile] : layer.getTileObjects())
				{
					string tilesetName = tile.getTile()->getTileset()->getName();
					Rect drawingRect = tile.getDrawingRect();
					Vector2f position = tile.getPosition();

					// In order to draw from a tileset, we need to clip the tileset and get only the tile itself
					// The tile's drawing rect contains the tile position and size in the tileset
					SDL_Rect clipRect = { drawingRect.x, drawingRect.y, drawingRect.width, drawingRect.height };
					SDL_Rect positionRect;

					positionRect.x = position.x-200;
					positionRect.y = position.y;
					positionRect.w = clipRect.w;
					positionRect.h = clipRect.h;

					// Push event
					BlitData* data = new BlitData;
					SDL_Event blitEvent;

					data->texture = tilesets[tilesetName];
					data->srcRect = clipRect;
					data->destRect = positionRect;

					blitEvent.type = BLIT;
					blitEvent.user.code = TILE_BLIT;
					blitEvent.user.data1 = data;
					blitEvent.user.data2 = 0;

					SDL_PushEvent(&blitEvent);
				}
			}
		}
	}
	else {
		std::cout << "cant parse" << "\n";
	}
}