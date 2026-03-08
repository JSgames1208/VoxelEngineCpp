#include "engine/renderer/block/FaceBakery.h"
#include "engine/texture/TextureAtlas.h"

BakedQuad FaceBakery::bakeQuad(vec3 min, vec3 max, Direction dir, BlockType type)
{
	float x0 = min.x;
	float y0 = min.y;
	float z0 = min.z;

	float x1 = max.x;
	float y1 = max.y;
	float z1 = max.z;

    AtlasTile tile = TextureAtlas::blockUVs[type];

	float u0 = tile.u0, v0 = tile.v0;
	float u1 = tile.u1, v1 = tile.v1;

	BakedQuad quad;
	quad.direction = dir;

    if (dir == Direction::UP)
    {
        quad.vertices[0] = bakeVertex(x0, y1, z0, u0, v0);
        quad.vertices[1] = bakeVertex(x1, y1, z0, u1, v0);
        quad.vertices[2] = bakeVertex(x1, y1, z1, u1, v1);
        quad.vertices[3] = bakeVertex(x0, y1, z1, u0, v1);
    }
    else if (dir == Direction::DOWN)
    {
        quad.vertices[0] = bakeVertex(x0, y0, z1, u0, v0);
        quad.vertices[1] = bakeVertex(x1, y0, z1, u1, v0);
        quad.vertices[2] = bakeVertex(x1, y0, z0, u1, v1);
        quad.vertices[3] = bakeVertex(x0, y0, z0, u0, v1);
    }
    else if (dir == Direction::NORTH)
    {
        quad.vertices[0] = bakeVertex(x0, y0, z0, u0, v0);
        quad.vertices[1] = bakeVertex(x1, y0, z0, u1, v0);
        quad.vertices[2] = bakeVertex(x1, y1, z0, u1, v1);
        quad.vertices[3] = bakeVertex(x0, y1, z0, u0, v1);
    }
    else if (dir == Direction::SOUTH)
    {
        quad.vertices[0] = bakeVertex(x1, y0, z1, u0, v0);
        quad.vertices[1] = bakeVertex(x0, y0, z1, u1, v0);
        quad.vertices[2] = bakeVertex(x0, y1, z1, u1, v1);
        quad.vertices[3] = bakeVertex(x1, y1, z1, u0, v1);
    }
    else if (dir == Direction::WEST)
    {
        quad.vertices[0] = bakeVertex(x0, y0, z1, u0, v0);
        quad.vertices[1] = bakeVertex(x0, y0, z0, u1, v0);
        quad.vertices[2] = bakeVertex(x0, y1, z0, u1, v1);
        quad.vertices[3] = bakeVertex(x0, y1, z1, u0, v1);
    }
    else if (dir == Direction::EAST)
    {
        quad.vertices[0] = bakeVertex(x1, y0, z0, u0, v0);
        quad.vertices[1] = bakeVertex(x1, y0, z1, u1, v0);
        quad.vertices[2] = bakeVertex(x1, y1, z1, u1, v1);
        quad.vertices[3] = bakeVertex(x1, y1, z0, u0, v1);
    }

    return quad;
}

Vertex FaceBakery::bakeVertex(float x, float y, float z, float u, float v)
{
	return {
		{x, y, z},
		{u, v}
	};
}