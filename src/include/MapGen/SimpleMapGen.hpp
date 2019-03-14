#ifndef MAPGEN_SIMPLEMAPGEN_HPP
#define MAPGEN_SIMPLEMAPGEN_HPP

#include <time.h>
#include <stdlib.h>

#include "MapGen/MapGen.hpp"
#include "Graphics/Tilemap.hpp"

class SimpleMapGenerator : public MapGenerator {
	
	void generateBlends(Tilemap &tilemap);
	
public:
	
	SimpleMapGenerator();
	SimpleMapGenerator(MapGenerator::SeedType seed);
	
	void blend(Tilemap &tilemap);
	
	virtual void generate(Tilemap &tilemap);
};

#endif
