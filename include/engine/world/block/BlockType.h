#pragma once
#include <unordered_map>
#include <iostream>

#include <functional>

enum class BlockType
{
	AIR,
	BEDROCK,
	COBBLESTONE,
	DIRT,
	GRASS_BLOCK,
	ICE,
	LOG,
	OBSIDIAN,
	SNOW_BLOCK,
	STONE,
	GRAVEL,
	LEAVES,
	SAND,
};

namespace std {
	template <>
	struct hash<BlockType> {
		inline std::size_t operator()(const BlockType& bt) const noexcept {
			return static_cast<std::size_t>(bt);
		}
	};
}