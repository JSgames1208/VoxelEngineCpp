#include "engine/world/block/BlockHelpers.h"
#include <unordered_map>

BlockType stringToBlockType(const std::string& name)
{
    static const std::unordered_map<std::string, BlockType> mapping = {
        {"grass_block", BlockType::GRASS_BLOCK},
        {"dirt", BlockType::DIRT},
        {"stone", BlockType::STONE},
        {"log", BlockType::LOG},
        {"bedrock", BlockType::BEDROCK},
        {"gravel", BlockType::GRAVEL},
        {"ice", BlockType::ICE},
        {"sand", BlockType::SAND},
        {"obsidian", BlockType::OBSIDIAN},
        {"snow_block", BlockType::SNOW_BLOCK},
        {"cobblestone", BlockType::COBBLESTONE},
        {"leaves", BlockType::LEAVES },
        {"air", BlockType::AIR }
    };

    auto it = mapping.find(name);
    if (it != mapping.end())
        return it->second;

    // default or error
    return BlockType::AIR;
}