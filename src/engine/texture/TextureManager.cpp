#include "engine/texture/TextureManager.h"

TextureID TextureManager::add(std::unique_ptr<Texture> texture)
{
	TextureID id = nextID++;
	textures[id] = std::move(texture);
	return id;
}

Texture& TextureManager::get(TextureID id)
{
	return *textures.at(id);
}