// 3dEngine.cpp : Defines the entry point for the application.
//

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "engine/Engine.h"
#include "engine/VoxelGame.h"

int main()
{
	Engine engine;
	VoxelGame game;

	return engine.run();
}
