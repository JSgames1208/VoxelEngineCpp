#include "engine/texture/TextureAtlas.h"
#include "engine/texture/TextureAtlasHelpers.h"

using namespace TextureAtlasHelpers;

std::unordered_map<BlockType, BlockUV> TextureAtlas::blockUVs = {
    { BlockType::GRASS_BLOCK, makeTopBottomSide(
        {0.5f, 0.75f, 0.75f, 1.0f},
        {0.0f, 0.0f, 0.25f, 0.25f},
        {0.25f, 0.75f, 0.5f, 1.0f}
    )},
    { BlockType::DIRT, makeUniformTile(
        {0.0f, 0.0f, 0.25f, 0.25f}
    )},
    { BlockType::STONE, makeUniformTile(
        {0.5f, 0.0f, 0.75f, 0.25f}
    )},
    { BlockType::LOG, makeSide(
        {0.25f, 0.0f, 0.5f, 0.25f},
        {0.25f, 0.25f, 0.5f, 0.5f}
    )},
    { BlockType::STONE, makeUniformTile(
        {0.5f, 0.0f, 0.75f, 0.25f}
    )},
    { BlockType::BEDROCK, makeUniformTile(
        {0.0f, 0.75f, 0.25f, 1.0f}
    )},
    { BlockType::GRAVEL, makeUniformTile(
        {0.75f, 0.75f, 1.0f, 1.0f}
    )},
    { BlockType::ICE, makeUniformTile(
        {0.25f, 0.5f, 0.5f, 0.75f}
    )}
};