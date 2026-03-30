#pragma once
#include <vector>
#include <memory>
#include <glad/glad.h>
#include "engine/renderer/block/BakedQuad.h"
#include "engine/renderer/block/FaceBakery.h"
#include "engine/world/Chunk.h"
#include "engine/mesh/Mesh.h"
#include <chrono>

class ChunkRenderer
{
public:
	ChunkRenderer() = default;

	std::unique_ptr<Mesh> createMeshFromQuads(const std::vector<BakedQuad>& quads)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		unsigned int indexOffset = 0;

		for (const auto& quad : quads)
		{
			for (int i = 0; i < 4; i++)
			{
				auto pos = quad.getPosition(i);
				auto uv = quad.getUV(i);
				auto ao = quad.getAO(i);

				vertices.push_back(pos.x);
				vertices.push_back(pos.y);
				vertices.push_back(pos.z);

				vertices.push_back(Directions::get(quad.direction).vector.x);
				vertices.push_back(Directions::get(quad.direction).vector.y);
				vertices.push_back(Directions::get(quad.direction).vector.z);

				vertices.push_back(uv.x);
				vertices.push_back(uv.y);

				vertices.push_back(ao);
			}

			indices.push_back(indexOffset);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 1);

			indices.push_back(indexOffset);
			indices.push_back(indexOffset + 3);
			indices.push_back(indexOffset + 2);

			indexOffset += 4;
		}

		auto mesh = std::make_unique<Mesh>();
		mesh->setData(vertices, indices);

		return mesh;
	}
};