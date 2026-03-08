#include "engine/mesh/MeshManager.h"

MeshID MeshManager::add(std::unique_ptr<Mesh> mesh)
{
	MeshID id = nextID++;
	meshes[id] = std::move(mesh);
	return id;
}

Mesh& MeshManager::get(MeshID id)
{
	return *meshes.at(id);
}