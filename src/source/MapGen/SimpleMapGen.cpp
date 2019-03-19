#include "MapGen/SimpleMapGen.hpp"

SimpleMapGenerator::SimpleMapGenerator():
	MapGenerator(time(nullptr) + rand() + (17 * rand() % 1723) * (rand() % 2 ? 1 : -1))
{
	
}

SimpleMapGenerator::SimpleMapGenerator(MapGenerator::SeedType seed):
	MapGenerator(seed)
{
	
}

static inline int neighbors_dirt(Tilemap &tm, int ox, int oy) {
	int cnt = 0;
	for (int y = -1; y <= 1; ++y) {
		for (int x = -1; x <= 1; ++x) {
			if (x == 0 && y == 0) continue;
			int id = tm.getTileID(ox + x, oy + y);
			if (id >= 10 && id < 14) ++cnt;
		}
	}
	return cnt;
}

void SimpleMapGenerator::generateBlends(Tilemap &tilemap) {
	for (int y = 1; y < tilemap.getWidth() - 1; ++y) {
		for (int x = 1; x < tilemap.getHeight() - 1; ++x) {
			int id = tilemap.getTileID(x, y);
			tilemap.setBlendTileID(x, y, 5, 0);
			
			#define is_dirt(id) (id >= 10 && id < 14)
			#define is_3dirt(id0, id1, id2) (is_dirt(id0) && is_dirt(id1) && is_dirt(id2))
			
			int above = tilemap.getTileID(x, y - 1);
			int below = tilemap.getTileID(x, y + 1);
			int left  = tilemap.getTileID(x - 1, y);
			int right = tilemap.getTileID(x + 1, y);
			int nw = tilemap.getTileID(x - 1, y - 1);
			int ne = tilemap.getTileID(x + 1, y - 1);
			int sw = tilemap.getTileID(x - 1, y + 1);
			int se = tilemap.getTileID(x + 1, y + 1);
			
			if (!is_dirt(id)) {
				if (is_dirt(above)) {
					tilemap.setBlendTileID(x, y, 8, above, 0.0f);
				} else if (is_dirt(below)) {
					tilemap.setBlendTileID(x, y, 8, below, 3.1415926f);
				} else if (is_dirt(left)) {
					tilemap.setBlendTileID(x, y, 8, left, 3.1415926f * 1.5f);
				} else if (is_dirt(right)) {
					tilemap.setBlendTileID(x, y, 8, right, 3.1415926f * 0.5f);
				} else if (is_dirt(nw)) {
					tilemap.setBlendTileID(x, y, 7, nw, 0.0f);
				} else if (is_dirt(ne)) {
					tilemap.setBlendTileID(x, y, 7, ne, 3.1415926f * 0.5f);
				} else if (is_dirt(sw)) {
					tilemap.setBlendTileID(x, y, 7, sw, 3.1415926f * 1.5f);
				} else if (is_dirt(se)) {
					tilemap.setBlendTileID(x, y, 7, se, 3.1415926f);
				}
				
				if (is_3dirt(above, left, left)) {
					tilemap.setBlendTileID(x, y, 17, left, 0.0f);
				} else if (is_3dirt(above, right, right)) {
					tilemap.setBlendTileID(x, y, 17, right, 3.1415926f * 0.5f);
				} else if (is_3dirt(below, left, below)) {
					tilemap.setBlendTileID(x, y, 17, below, 3.1415926f * 1.5f);
				} else if (is_3dirt(below, right, right)) {
					tilemap.setBlendTileID(x, y, 17, right, 3.1415926f);
				}
				
				if (is_3dirt(above, left, below)) {
					tilemap.setBlendTileID(x, y, 19, left, 3.1415926f * 1.0f);
				} else if (is_3dirt(above, right, below)) {
					tilemap.setBlendTileID(x, y, 19, right, 3.1415926f * 0.0f);
				} else if (is_3dirt(left, above, right)) {
					tilemap.setBlendTileID(x, y, 19, above, 3.1415926f * 1.5f);
				} else if (is_3dirt(left, below, right)) {
					tilemap.setBlendTileID(x, y, 19, right, 3.1415926f * 0.5f);
				}
				
				if (is_dirt(above) && is_dirt(below) && !is_dirt(left) && !is_dirt(right)) {
					tilemap.setBlendTileID(x, y, 18, above, 0.0f);
				} else if (is_dirt(left) && is_dirt(right) && !is_dirt(above) && !is_dirt(below)) {
					tilemap.setBlendTileID(x, y, 18, left, 3.1415926f * 0.5f);
				}
				
				if (is_dirt(ne) && is_dirt(se) && !is_dirt(right) && !is_dirt(above) && !is_dirt(below)) {
					tilemap.setBlendTileID(x, y, 18, left, 3.1415926f * 0.5f);
				}
				
				int dirt_neighs = neighbors_dirt(tilemap, x, y);
				
				if (is_dirt(nw) && is_dirt(se) && dirt_neighs == 2) {
					tilemap.setBlendTileID(x, y, 28, nw, 3.1415926f * 0.0f);
				} else if (is_dirt(ne) && is_dirt(sw) && dirt_neighs == 2) {
					tilemap.setBlendTileID(x, y, 28, ne, 3.1415926f * 0.5f);
				}
				if (dirt_neighs == 4 && is_dirt(ne) && is_dirt(nw) && is_dirt(se) && is_dirt(sw)) {
					tilemap.setBlendTileID(x, y, 29, ne, 3.1415926f * 0.0f);
				}
				
				if (is_dirt(above) && is_3dirt(left, right, below)) {
					tilemap.setBlendTileID(x, y, 9, above, 0.0f);
				}
				
			
			}
		}
	}
}

void SimpleMapGenerator::blend(Tilemap &tilemap) {
	generateBlends(tilemap);
}

void SimpleMapGenerator::generate(Tilemap &tilemap) {
	FastNoise perlin;
	perlin.SetNoiseType(FastNoise::PerlinFractal);
	perlin.SetSeed(getSeed());
	perlin.SetFrequency(0.05f);
	
	for (int y = 0; y < tilemap.getWidth(); ++y) {
		for (int x = 0; x < tilemap.getHeight(); ++x) {
			int id = 0;
			float noise = perlin.GetNoise(x - tilemap.getWidth() * 0.5, y - tilemap.getHeight() * 0.5) * 0.5f + 0.5f;
			
			if (noise > 0.5f) {
				id = 30 + rand() % 4;
			} else {
				id = 10 + rand() % 4;
			}
			tilemap.setTileID(x, y, id);
			
			if (x == 0 || y == 0 || x == tilemap.getWidth() - 1 || y == tilemap.getHeight() - 1) {
				tilemap.setBlendTileID(x, y, 9, 0);
			} else {
				tilemap.setBlendTileID(x, y, 5, 0);
			}
		}
	}
	
	blend(tilemap);
	tilemap.updateTileIDs();
}
