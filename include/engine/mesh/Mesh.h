#pragma once
#include <vector>
#include <memory>
#include "glad/glad.h"
#include "engine/mesh/ThreadedMeshCreator.h"
#include "engine/mesh/ThreadedChunkMesher.h"

class Mesh
{
public:

	Mesh(size_t maxVertices = 100000, size_t maxIndices = 100000);
	~Mesh();

	size_t indexCount;

	void setData(std::unique_ptr<MeshData> meshData);
	void draw();
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	size_t maxVertices;
	size_t maxIndices;
};
