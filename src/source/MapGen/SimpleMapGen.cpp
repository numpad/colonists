#include "MapGen/SimpleMapGen.hpp"

SimpleMapGenerator::SimpleMapGenerator():
	MapGenerator(time(nullptr))
{
	
}

void SimpleMapGenerator::generate(Tilemap &tilemap) {
	FastNoise perlin;
	perlin.SetNoiseType(FastNoise::PerlinFractal);
	perlin.SetSeed(getSeed());
	perlin.SetFrequency(0.05f);
	
	int tW, tH;
	tilemap.getSize(&tW, &tH);
	for (int y = 0; y < tW; ++y) {
		for (int x = 0; x < tH; ++x) {
			int id = 0;
			float noise = perlin.GetNoise(x, y) * 0.5f + 0.5f;
			
			if (noise > 0.5f) {
				id = 30 + rand() % 4;
			} else {
				id = 10 + rand() % 4;
			}
			tilemap.setTileID(x, y, id);
			
			tilemap.setBlendTileID(x, y, 5, 20);
			if (y > 0) {
				int above = tilemap.getTileID(x, y - 1);
				if (above >= 10 && above < 14) {
					if (id < 10 || id >= 14)
						tilemap.setBlendTileID(x, y, 8, 10);
				}
			} 
		}
	}
	
	tilemap.updateTileIDs();
}
