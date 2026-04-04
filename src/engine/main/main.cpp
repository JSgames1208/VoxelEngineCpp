// main.cpp : Defines the entry point for the application.
//

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "engine/Engine.h"
#include "engine/VoxelGame.h"
#include "engine/main/GameConfig.h"
#include "engine/platform/DisplayData.h"

int main()
{
	Engine engine;

    int width = 2400;
    int height = 1800;

    GameConfig gameConfig(DisplayData(width, height));

	VoxelGame game(gameConfig);

	return engine.run(game);
}
