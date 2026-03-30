#pragma once
#include "engine/renderer/block/BakedQuad.h"
#include "engine/world/block/BlockType.h"
#include "engine/core/Direction.h"
#include "engine/texture/TextureAtlas.h"
#include <memory>

class FaceBakery
{
public:
	FaceBakery() = default;

	BakedQuad bakeQuad(vec3 min, vec3 max, Direction dir, BlockType type, float ao[4], TextureAtlas* atlas);
	Vertex bakeVertex(float x, float y, float z, float u, float v, float ao);
private:
};