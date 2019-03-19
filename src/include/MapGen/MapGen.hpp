#ifndef MAPGEN_MAPGEN_HPP
#define MAPGEN_MAPGEN_HPP

#include "Graphics/Tilemap.hpp"

class MapGenerator {
public:	
	using SeedType = unsigned long int;
	
private:
	SeedType seed = 0;
	
public:
	
	MapGenerator(SeedType seed = 0);
	virtual ~MapGenerator();
	
	SeedType getSeed() const;
	
	virtual void generate(Tilemap &tilemap) =0;
};

#endif
