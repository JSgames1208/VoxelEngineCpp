#pragma once
#include <vector>
#include "glad/glad.h"

class Mesh
{
public:

	Mesh(size_t maxVertices = 100000, size_t maxIndices = 100000);
	~Mesh();

	size_t indexCount;

	void setData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	void draw();
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	size_t maxVertices;
	size_t maxIndices;
};
