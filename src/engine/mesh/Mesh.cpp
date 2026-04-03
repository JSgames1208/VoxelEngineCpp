#include <iostream>
#include "engine/mesh/Mesh.h"
#include "glad/glad.h"

Mesh::Mesh(size_t maxVertices, size_t maxIndices)
	: indexCount(0)
	, maxVertices(maxVertices)
	, maxIndices(maxIndices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

	// Vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// Done
	glBindVertexArray(0); // unbind VAO
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::setData(std::unique_ptr<MeshData> meshData) {
	indexCount = meshData->indices.size();

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (meshData->vertices.size() > maxVertices) {
		maxVertices = meshData->vertices.size();
		glBufferData(GL_ARRAY_BUFFER, meshData->vertices.size() * sizeof(float), meshData->vertices.data(), GL_STATIC_DRAW);
	} else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, meshData->vertices.size() * sizeof(float), meshData->vertices.data());
	}

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	if (meshData->indices.size() > maxIndices)
	{
		maxIndices = meshData->indices.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indices.size() * sizeof(unsigned int), meshData->indices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, meshData->indices.size() * sizeof(unsigned int), meshData->indices.data());
	}
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}