#include "MapGen/PerlinMapGen.hpp"

static inline float clamp1(float v) {
	return fmaxf(0.0f, fminf(1.0f, v));
}

PerlinMapGen::PerlinMapGen(int seed):
	MapGenerator(seed),
	biomeConditions(0)
{
	pnoise.SetNoiseType(FastNoise::Perlin);
	pnoise.SetSeed(seed);
	pnoise.SetFrequency(1.0f);
}

void PerlinMapGen::generate(Tilemap &tilemap) {
	for (int y = 0; y < tilemap.getHeight(); ++y) {
		for (int x = 0; x < tilemap.getWidth(); ++x) {
			float elevation = getElevationAt(x, y);
			
			tilemap.setBlendTileID(x, y, 5, 9);
			tilemap.setTileID(x, y, biomeConditions.at(elevation));
		}
	}
	
	tilemap.updateTileIDs();
}

void PerlinMapGen::setOctaves(int octaves) {
	this->octaves = octaves;
}

void PerlinMapGen::setExponent(float exp) {
	this->exponent = exp;
}

void PerlinMapGen::setFrequency(float freq) {
	pnoise.SetFrequency(freq);
}

void PerlinMapGen::setNoiseShift(float x, float y) {
	xOff = x;
	yOff = y;
}

RangeDecider<int> &PerlinMapGen::getTileConditions() {
	return biomeConditions;
}

float PerlinMapGen::getElevationAt(float x, float y) {
	return clamp1(powf(noise(x, y, octaves) * 0.5f + 0.5f, exponent));
}

/////////////
// PRIVATE //
/////////////

float PerlinMapGen::noise(float x, float y, int octaves) {
	float e = 0.0f;
	
	for (int i = 0; i < octaves; ++i) {
		float f = powf(2.0f, (float)i);
		float s = 1.0f / f;
		e += s * pnoise.GetNoise(x * f + xOff, y * f + yOff);
		
	}
	//printf(" = %g\n\n", e);
	
	return e;
}
