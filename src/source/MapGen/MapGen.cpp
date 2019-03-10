#include "MapGen/MapGen.hpp"

MapGenerator::MapGenerator(int seed):
	seed(seed)
{
	
}

int MapGenerator::getSeed() const {
	return seed;
}