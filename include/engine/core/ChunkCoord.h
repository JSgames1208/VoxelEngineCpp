#pragma once

struct ChunkCoord
{
	ChunkCoord(int _x, int _z) 
	{
		x = _x;
		z = _z;
	}

	int x;
	int z;

    bool operator==(const ChunkCoord& other) const
    {
        return x == other.x && z == other.z;
    }
};

namespace std {
    template<>
    struct hash<ChunkCoord> {
        size_t operator()(const ChunkCoord& coord) const noexcept {
            size_t h1 = std::hash<int>{}(coord.x);
            size_t h2 = std::hash<int>{}(coord.z);
            return h1 ^ (h2 << 1); // simple combination
        }
    };
}