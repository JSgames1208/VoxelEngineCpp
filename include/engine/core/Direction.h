#pragma once
#include <string_view>
#include <glm/glm.hpp>
#include <array>

enum class Direction : uint8_t
{
	DOWN = 0, UP, NORTH, SOUTH, WEST, EAST
};

struct DirectionInfo {
	Direction type;
	Direction opposite;
	std::string_view name;
	glm::vec3 vector;
};

class Directions {
public:
	static const DirectionInfo& get(Direction dir) {
		static const std::array<DirectionInfo, 6> table{ {
			{ Direction::DOWN,  Direction::UP,    "down",  {0.0f, -1.0f, 0.0f} },
			{ Direction::UP,    Direction::DOWN,  "up",    {0.0f, 1.0f, 0.0f}  },
			{ Direction::NORTH, Direction::SOUTH, "north", {0.0f, 0.0f, -1.0f} },
			{ Direction::SOUTH, Direction::NORTH, "south", {0.0f, 0.0f, 1.0f}  },
			{ Direction::WEST,  Direction::EAST,  "west",  {-1.0f, 0.0f, 0.0f} },
			{ Direction::EAST,  Direction::WEST,  "east",  {1.0f, 0.0f, 0.0f}  }
		} };

		return table[static_cast<size_t>(dir)];
	}

	static Direction opposite(Direction d) { return get(d).opposite; }
	static glm::vec3 vec(Direction d) { return get(d).vector; }
};