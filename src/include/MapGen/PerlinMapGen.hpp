#ifndef MAPGEN_PERLINMAPGEN_HPP
#define MAPGEN_PERLINMAPGEN_HPP

#include "MapGen/MapGen.hpp"

#include <math.h>
#include "Graphics/Tilemap.hpp"
#include <FastNoise/FastNoise.h>

class PerlinMapGen : public MapGenerator {
	FastNoise pnoise;
	
	float noise(float x, float y, int octaves);
	
	int octaves = 1;
	float exponent = 1.0f;
	
public:
	
	PerlinMapGen(int seed);
	
	virtual void generate(Tilemap &tilemap);
	
	void setOctaves(int octaves);
	void setExponent(float exp);
	void setFrequency(float freq);
	
	float getElevationAt(float x, float y);
};

#endif
