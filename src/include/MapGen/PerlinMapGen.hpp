#ifndef MAPGEN_PERLINMAPGEN_HPP
#define MAPGEN_PERLINMAPGEN_HPP

#include "MapGen/MapGen.hpp"

#include <math.h>
#include <FastNoise/FastNoise.h>
#include "Graphics/Tilemap.hpp"
#include "System/RangeDecider.hpp"

class PerlinMapGen : public MapGenerator {
	FastNoise pnoise;
	
	RangeDecider<int> biomeConditions;
	
	float noise(float x, float y, int octaves);
	
	int octaves = 1;
	float exponent = 1.0f;
	
	float xOff = 0.0f, yOff = 0.0f;
public:
	
	PerlinMapGen(int seed);
	
	virtual void generate(Tilemap &tilemap);
	
	void setOctaves(int octaves);
	void setExponent(float exp);
	void setFrequency(float freq);
	void setNoiseShift(float x, float y);
	
	RangeDecider<int> &getTileConditions();
	
	float getElevationAt(float x, float y);
};

#endif
