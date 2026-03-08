#pragma once
#include "engine/mesh/Mesh.h"
#include <unordered_map>
#include <memory>

using MeshID = unsigned int;

class MeshManager
{
public:
	MeshID add(std::unique_ptr<Mesh> mesh);
	Mesh& get(MeshID id);
private:
	std::unordered_map<MeshID, std::unique_ptr<Mesh>> meshes;
	MeshID nextID = 1;
};