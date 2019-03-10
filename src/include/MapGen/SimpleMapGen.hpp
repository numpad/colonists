#ifndef MAPGEN_SIMPLEMAPGEN_HPP
#define MAPGEN_SIMPLEMAPGEN_HPP

#include <time.h>

#include "MapGen/MapGen.hpp"
#include "Graphics/Tilemap.hpp"

class SimpleMapGenerator : public MapGenerator {
	
public:
	
	SimpleMapGenerator();
	
	virtual void generate(Tilemap &tilemap);
};

#endif
