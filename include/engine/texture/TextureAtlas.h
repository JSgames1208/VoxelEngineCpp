#pragma once
#include <unordered_map>
#include "engine/world/block/BlockType.h"

struct AtlasTile {
	float u0, v0, u1, v1;
};

class TextureAtlas {
public:
	static std::unordered_map<BlockType, AtlasTile> blockUVs;
};