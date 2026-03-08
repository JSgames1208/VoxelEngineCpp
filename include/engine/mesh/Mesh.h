#pragma once
#include <vector>

class Mesh
{
public:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	Mesh();
	~Mesh();

	size_t indexCount;

	void setData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	void draw();
};