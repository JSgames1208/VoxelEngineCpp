#pragma once
#include <glm/glm.hpp>
#include "engine/core/Direction.h"

using vec3 = glm::vec3;
using vec2 = glm::vec2;

struct Vertex
{
	vec3 position;
	vec2 uv;
};

struct BakedQuad
{
	Vertex vertices[4];
	Direction direction;

	const vec3 getPosition(int i) const
	{
		return vertices[i].position;
	}

	const vec2 getUV(int i) const
	{
		return vertices[i].uv;
	}
};