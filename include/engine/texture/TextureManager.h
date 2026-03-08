#pragma once
#include "engine/texture/Texture.h"
#include <unordered_map>
#include <memory>

using TextureID = unsigned int;

class TextureManager
{
public:
	TextureID add(std::unique_ptr<Texture> texture);
	Texture& get(TextureID id);
private:
	std::unordered_map<TextureID, std::unique_ptr<Texture>> textures;
	TextureID nextID = 1;
};