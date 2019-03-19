#include "MapGen/MapGen.hpp"

MapGenerator::MapGenerator(MapGenerator::SeedType seed):
	seed(seed)
{
	
}

MapGenerator::~MapGenerator() {
	
}

MapGenerator::SeedType MapGenerator::getSeed() const {
	return seed;
}