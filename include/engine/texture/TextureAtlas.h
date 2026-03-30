#pragma once
#include <iostream>
#include <unordered_map>
#include <array>
#include <mutex>
#include "engine/world/block/BlockType.h"

struct AtlasUV {
	float u0, v0, u1, v1;
};

inline AtlasUV getUVFromIndex(int index, int atlasSize)
{
	float tileSize = 1.0f / atlasSize;

	int x = index % atlasSize;
	int y = index / atlasSize;

	float u0 = x * tileSize;
	float v1 = 1 - y * tileSize;
	float u1 = u0 + tileSize;
	float v0 = v1 - tileSize;

	return { u0, v0, u1, v1 };
}

class TextureAtlas {
public:
	bool loadFromJSON();

	int getTextureIndex(const std::string& textureName);
	const std::array<int, 6> getBlockFaces(BlockType type);
private:
	std::unordered_map<std::string, int> textureIndices;
	std::unordered_map<BlockType, std::array<int, 6>> blockFaceIndices;

	std::recursive_mutex atlasMutex;
};