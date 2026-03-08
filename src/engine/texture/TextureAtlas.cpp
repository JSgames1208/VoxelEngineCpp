#include "engine/texture/TextureAtlas.h"

std::unordered_map<BlockType, AtlasTile> TextureAtlas::blockUVs = {
	{ BlockType::SAND, {0.75f, 0.5f, 1.0f, 0.75f}},
	{ BlockType::BRICKS, {0.0f, 0.5f, 0.25f, 0.75f}},
	{ BlockType::BEDROCK, {0.0f, 0.75f, 0.25f, 1.0f}},
};