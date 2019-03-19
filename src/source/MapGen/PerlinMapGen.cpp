#include "MapGen/PerlinMapGen.hpp"

PerlinMapGen::PerlinMapGen(int seed):
	MapGenerator(seed)
{
	pnoise.SetSeed(seed);
	pnoise.SetFrequency(1.0f);
}

void PerlinMapGen::generate(Tilemap &tilemap) {
	//printf("noise = %g\n", noise(3, 0, octaves));
	
	for (int y = 0; y < tilemap.getHeight(); ++y) {
		for (int x = 0; x < tilemap.getWidth(); ++x) {
			float elevation = noise(x, y, octaves);
			
			tilemap.setBlendTileID(x, y, 5, 9);
			if (elevation < 0.5f) {
				tilemap.setTileID(x, y, 40);
			} else {
				tilemap.setTileID(x, y, 30);
			}
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

float PerlinMapGen::getElevationAt(float x, float y) {
	return noise(x, y, octaves);
}

/////////////
// PRIVATE //
/////////////

float PerlinMapGen::noise(float x, float y, int octave) {
	if (!octave) return 0.0f;
	
	float oct = pow(2, octave - 1);
	float n = (1.0f / oct) * powf(pnoise.GetNoise(x * oct, y * oct) * 0.5f + 0.5f, exponent);
	//printf("%g * noise(x * %d, y * %d) = %g\n", 1.0f / oct, (int)oct, (int)oct, n);
	
	return n + noise(x, y, octave - 1);
}
