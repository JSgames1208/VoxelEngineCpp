#include "engine/texture/TextureAtlas.h"
#include "engine/texture/TextureAtlasHelpers.h"
#include "engine/world/block/BlockType.h"
#include "engine/world/block/BlockHelpers.h"
#include "json.hpp"
#include <fstream>

using nlohmann::json;

bool TextureAtlas::loadFromJSON()
{
    std::lock_guard<std::recursive_mutex> lock(atlasMutex);

	std::ifstream texFile("data/textures.json");
    if (!texFile.is_open()) {
        std::cout << "Failed to open textures.json\n";
        return false;
    }
    std::cout << "textures.json opened successfully\n";
	json texJson;
	texFile >> texJson;
    std::cout << "Parsed textures.json\n";
    for (auto& [name, index] : texJson.items()) {
        textureIndices[name] = index;
    }

    std::ifstream blockFile("data/blocks.json");
    if (!blockFile.is_open()) {
        std::cout << "Failed to open blocks.json\n";
        return false;
    }
    std::cout << "blocks.json opened successfully" << std::endl;
    json blockJson;
    try {
        blockFile >> blockJson;
        std::cout << "Parsed blocks.json\n";
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error in blocks.json: " << e.what() << "\n";
        return false;
    }
    std::cout << "Parsed blocks.json" << std::endl;
    for (auto& [blockName, faces] : blockJson.items()) {
        BlockType type = stringToBlockType(blockName);
        std::array<int, 6> faceIDs;
        for (int i = 0; i < 6; ++i) {
            std::string texName = faces[i];

            auto it = textureIndices.find(texName);
            if (it == textureIndices.end()) {
                std::cout << "Missing texture: " << texName << "\n";
                faceIDs[i] = 0;
            }
            else {
                faceIDs[i] = it->second;
            }
        }
        blockFaceIndices[type] = faceIDs;
        if (type == BlockType::AIR && blockName != "air") {
            std::cout << "Invalid block name: " << blockName << "\n";
        }
    }

    std::cout << "--- Loaded Textures ---\n";
    for (auto& [name, index] : textureIndices) {
        std::cout << name << " => " << index << "\n";
    }

    std::cout << "--- Loaded Block Faces ---\n";

    for (int i = 0; i <= static_cast<int>(BlockType::SAND); i++)
    {
        auto faces = this->getBlockFaces(static_cast<BlockType>(i));
        std::cout << i << ": " << faces[0] << "\n";
    }

    return !textureIndices.empty() && !blockFaceIndices.empty();
}

int TextureAtlas::getTextureIndex(const std::string& textureName)
{
    std::lock_guard<std::recursive_mutex> lock(atlasMutex);
    return textureIndices.at(textureName);
}

const std::array<int, 6> TextureAtlas::getBlockFaces(BlockType type)
{
    std::lock_guard<std::recursive_mutex> lock(atlasMutex);

    int t = static_cast<int>(type);

    if (t < 0 || t > static_cast<int>(BlockType::SAND)) {
        std::cout << "[ERROR] Invalid BlockType requested: " << t << std::endl;
        return { 0,0,0,0,0,0 };
    }

    if (blockFaceIndices.empty()) {
        std::cout << "[ERROR] blockFaceIndices is empty!\n";
        return { 0,0,0,0,0,0 };
    }

    auto it = blockFaceIndices.find(type);
    if (it == blockFaceIndices.end()) {
        std::cout << "[ERROR] BlockType not found in atlas: " << t << std::endl;
        return { 0,0,0,0,0,0 };
    }

    return it->second;
}