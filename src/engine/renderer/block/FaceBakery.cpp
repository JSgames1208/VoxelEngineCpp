#include "engine/renderer/block/FaceBakery.h"
#include "engine/texture/TextureAtlas.h"

BakedQuad FaceBakery::bakeQuad(vec3 min, vec3 max, Direction dir, BlockType type, float ao[4], TextureAtlas* atlas)
{
	float x0 = min.x;
	float y0 = min.y;
	float z0 = min.z;

	float x1 = max.x;
	float y1 = max.y;
	float z1 = max.z;

    if (!atlas) {
        std::cout << "[ERROR] atlas pointer is null! Cannot get block faces.\n";
        return {};
    }

    auto faces = atlas->getBlockFaces(type);

    int faceIndex;
    switch (dir)
    {
        case Direction::UP:    faceIndex = faces[0]; break;
        case Direction::DOWN:  faceIndex = faces[1]; break;
        case Direction::NORTH: faceIndex = faces[2]; break;
        case Direction::SOUTH: faceIndex = faces[3]; break;
        case Direction::WEST:  faceIndex = faces[4]; break;
        case Direction::EAST:  faceIndex = faces[5]; break;
        default: faceIndex = faces[2]; break;
    }

    AtlasUV uv = getUVFromIndex(faceIndex, 5);

    float u0 = uv.u0;
    float v0 = uv.v0;
    float u1 = uv.u1;
    float v1 = uv.v1;

	BakedQuad quad;
	quad.direction = dir;

    if (dir == Direction::UP)
    {
        quad.vertices[0] = bakeVertex(x0, y1, z0, u0, v0, ao[0]);
        quad.vertices[1] = bakeVertex(x1, y1, z0, u1, v0, ao[1]);
        quad.vertices[2] = bakeVertex(x1, y1, z1, u1, v1, ao[2]);
        quad.vertices[3] = bakeVertex(x0, y1, z1, u0, v1, ao[3]);
    }
    else if (dir == Direction::DOWN)
    {
        quad.vertices[0] = bakeVertex(x0, y0, z1, u0, v0, ao[0]);
        quad.vertices[1] = bakeVertex(x1, y0, z1, u1, v0, ao[1]);
        quad.vertices[2] = bakeVertex(x1, y0, z0, u1, v1, ao[2]);
        quad.vertices[3] = bakeVertex(x0, y0, z0, u0, v1, ao[3]);
    }
    else if (dir == Direction::NORTH)
    {
        quad.vertices[0] = bakeVertex(x0, y0, z0, u0, v0, ao[0]);
        quad.vertices[1] = bakeVertex(x1, y0, z0, u1, v0, ao[1]);
        quad.vertices[2] = bakeVertex(x1, y1, z0, u1, v1, ao[2]);
        quad.vertices[3] = bakeVertex(x0, y1, z0, u0, v1, ao[3]);
    }
    else if (dir == Direction::SOUTH)
    {
        quad.vertices[0] = bakeVertex(x1, y0, z1, u0, v0, ao[0]);
        quad.vertices[1] = bakeVertex(x0, y0, z1, u1, v0, ao[1]);
        quad.vertices[2] = bakeVertex(x0, y1, z1, u1, v1, ao[2]);
        quad.vertices[3] = bakeVertex(x1, y1, z1, u0, v1, ao[3]);
    }
    else if (dir == Direction::WEST)
    {
        quad.vertices[0] = bakeVertex(x0, y0, z1, u0, v0, ao[0]);
        quad.vertices[1] = bakeVertex(x0, y0, z0, u1, v0, ao[1]);
        quad.vertices[2] = bakeVertex(x0, y1, z0, u1, v1, ao[2]);
        quad.vertices[3] = bakeVertex(x0, y1, z1, u0, v1, ao[3]);
    }
    else if (dir == Direction::EAST)
    {
        quad.vertices[0] = bakeVertex(x1, y0, z0, u0, v0, ao[0]);
        quad.vertices[1] = bakeVertex(x1, y0, z1, u1, v0, ao[1]);
        quad.vertices[2] = bakeVertex(x1, y1, z1, u1, v1, ao[2]);
        quad.vertices[3] = bakeVertex(x1, y1, z0, u0, v1, ao[3]);
    }

    return quad;
}

Vertex FaceBakery::bakeVertex(float x, float y, float z, float u, float v, float ao)
{
	return {
		{x, y, z},
		{u, v},
        ao
	};
}