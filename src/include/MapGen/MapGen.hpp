#ifndef MAPGEN_MAPGEN_HPP
#define MAPGEN_MAPGEN_HPP

#include "Graphics/Tilemap.hpp"

class MapGenerator {
	int seed = 0;
	
public:
	
	MapGenerator(int seed = 0);
	
	int getSeed() const;
	
	virtual void generate(Tilemap &tilemap) =0;
};

#endif
