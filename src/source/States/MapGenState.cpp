#include "States/MapGenState.hpp"

MapGenState::MapGenState(Tilemap &tilemap):
	tilemap(tilemap)
{
	mapGens.push_back("Simple");
	mapGens.push_back("Perlin Noise");
}

MapGenState::~MapGenState() {
	heightmapPreview.destroy();
}

double MapGenState::generate(int w, int h) {
	tilemap.create(w, h);
	
	MapGenerator *generator = getMapGen();
	
	double begin_s = glfwGetTime();
	generator->generate(tilemap);
	double end_s = glfwGetTime();
	double dt = end_s - begin_s;
	tilemap.clearCache();
	
	delete generator;
	
	return dt;
}

void MapGenState::draw() {
	glClearColor(0.5, 0.67, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	static double genTime_s = -1.0;
	
	ImGui::BeginMainMenuBar();
	ImGui::MenuItem("World Generator");
	ImGui::EndMainMenuBar();
	
	if (ImGui::Begin("Generator Settings")) {
		ImGui::Combo("Generator", &currentMapGen, &mapGens[0], mapGens.size());
		
		if (ImGui::InputInt("Seed", &map_seed)) {
			renderHeightmapPreview();
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			map_seed = rand();
			if (currentMapGen == 1)
				renderHeightmapPreview();
		}
		
		ImGui::SliderInt("Size (in Tiles)", &map_size, 32, 512);
		if (genTime_s > 0.0)
			ImGui::Text("Generating took %.2f seconds.", genTime_s);
		
		if (ImGui::Button("Generate")) {
			generate(map_size, map_size);
			setFinished(true);
		}
	}
	ImGui::End();
	
	presetsWindow();
	
	if (currentMapGen == 1) {
		if (ImGui::Begin("Perlin noise settings")) {
			if (ImGui::SliderInt("Octaves", &genOctaves, 1, 8))
				renderHeightmapPreview();
			
			if (ImGui::SliderFloat("Redist", &genRedistExp, 0.0f, 10.0f))
				renderHeightmapPreview();
			
			if (ImGui::SliderFloat("Frequency", &genFreq, 0.001f, 0.05f))
				renderHeightmapPreview();
			
			if (ImGui::DragFloat2("Offset", shiftOffset, 0.75f)) {
				renderHeightmapPreview();
			}
		}
		ImGui::End();
		
		if (ImGui::Begin("Biomes")) {
			for (size_t i = 0; i < biomeTiles.size(); ++i) {
				char label[16];
				sprintf(label, "%d##%lu", biomeTiles[i], i);
				if (ImGui::SliderFloat(label, &biomeRanges[i], 0.0f, 1.0f))
					renderHeightmapPreview();
			}
			
			ImGui::Separator();
			
			static int tile = 0;
			ImGui::InputInt("", &tile);
			ImGui::SameLine();
			if (ImGui::Button("Add")) {
				biomeRanges.push_back(0.0f);
				biomeTiles.push_back(tile);
			}
			ImGui::SameLine();
			if (ImGui::Button("Clear")) {
				biomeRanges.clear();
				biomeTiles.clear();
			}
		}
		ImGui::End();
		
		if (ImGui::Begin("Elevation map preview")) {
			heightmapPreview.active(0);
			ImGui::Image((ImTextureID)(intptr_t)(GLuint)heightmapPreview,
				ImVec2(map_size, map_size));
		}
		ImGui::End();
	}
}

void MapGenState::addBiome(int tileID, float minRange) {
	biomeRanges.push_back(minRange);
	biomeTiles.push_back(tileID);
}

void MapGenState::presetsWindow() {
	if (currentMapGen == 1) {
		if (ImGui::Begin("Load preset")) {
			if (ImGui::Button("Swamp")) {
				map_seed = rand();
				genOctaves = 4;
				genRedistExp = 0.9f;
				genFreq = 0.02f;
				biomeRanges.clear();
				biomeTiles.clear();
				shiftOffset[0] = 0.0f;
				shiftOffset[1] = 0.0f;
				addBiome(41, 0.0f);
				addBiome(40, 0.22f);
				addBiome(30, 0.482f);
				addBiome(10, 0.681f);
				renderHeightmapPreview();
			}
			ImGui::SameLine();
			if (ImGui::Button("Valley")) {
				map_seed = rand();
				genOctaves = 5;
				genRedistExp = 0.889f;
				genFreq = 0.005f;
				biomeRanges.clear();
				biomeTiles.clear();
				shiftOffset[0] = 0.0f;
				shiftOffset[1] = 0.0f;
				addBiome(40, 0.0f);
				addBiome(20, 0.145f);
				addBiome(10, 0.325f);
				addBiome(30, 0.506f);
				renderHeightmapPreview();
			}
			if (ImGui::Button("Islands")) {
				map_seed = rand();
				genOctaves = 5;
				genRedistExp = 2.074f;;
				genFreq = 0.006f;
				biomeRanges.clear();
				biomeTiles.clear();
				shiftOffset[0] = 0.0f;
				shiftOffset[1] = 0.0f;
				addBiome(41, 0.0f);
				addBiome(40, 0.2f);
				addBiome(20, 0.265f);
				addBiome(10, 0.325f);
				addBiome(30, 0.506f);
				renderHeightmapPreview();
			}
		}
		ImGui::End();
	}
}

MapGenerator *MapGenState::getMapGen() {
	switch (currentMapGen) {
		case 0: {
			SimpleMapGenerator *sg = new SimpleMapGenerator(map_seed);
			return sg;
		}
		case 1: {
			PerlinMapGen *pg = new PerlinMapGen(map_seed);
			pg->setOctaves(genOctaves);
			pg->setExponent(genRedistExp);
			pg->setFrequency(genFreq);
			pg->setNoiseShift(shiftOffset[0], shiftOffset[1]);
			auto &rd = pg->getTileConditions();
			
			rd.clear();
			for (size_t i = 0; i < biomeTiles.size(); ++i) {
				float max = 1.0f;
				if (i < biomeTiles.size() - 1) max = biomeRanges.at(i + 1);
				
				rd.addRange(biomeTiles.at(i), biomeRanges.at(i), max);
			}
			
			return pg;
		}
		default: break;
	}
	
	return nullptr;
}

void MapGenState::renderHeightmapPreview() {
	
	std::vector<GLubyte> pixels;
	pixels.resize(map_size * map_size * 4);
	PerlinMapGen *gen = (PerlinMapGen *)getMapGen();
	gen->setNoiseShift(shiftOffset[0], shiftOffset[1]);
	for (int y = 0; y < map_size; ++y) {
		for (int x = 0; x < map_size; ++x) {
			float e = gen->getElevationAt(x + shiftOffset[0], y + shiftOffset[1]);
			int tileID = gen->getTileConditions().at(e);
			float r = 1.0f, g = 1.0f, b = 1.0f;
			
			switch (tileID) {
				case 10: r = 0.41f; g = 0.388f; b = 0.27f; break;
				case 20: r = 0.78f; g = 0.71f; b = 0.33f; break;
				case 30: r = 0.25f; g = 0.60f; b = 0.27f; break;
				case 40: r = 0.475f; g = 0.625f; b = 1.0f; break;
				case 41: r = 0.44f; g = 0.55f; b = 0.99f; break;
				default: r = 1.0f; g = 0.0f; b = 0.0f; break;
			};
			
			pixels.at((x + y * map_size) * 4 + 0) = ((e * 0.7f + 0.3f) * r * 255);
			pixels.at((x + y * map_size) * 4 + 1) = ((e * 0.7f + 0.3f) * g * 255);
			pixels.at((x + y * map_size) * 4 + 2) = ((e * 0.7f + 0.3f) * b * 255);
			pixels.at((x + y * map_size) * 4 + 3) = 255;
		}
	}
	
	if (heightmapPreview.getWidth() == map_size && heightmapPreview.getHeight() == map_size) {
		heightmapPreview.updateTextureFromMemory(&pixels[0]);
	} else {
		heightmapPreview.destroy();
		heightmapPreview.setTextureData(map_size, map_size, 4);
		heightmapPreview.loadTextureFromMemory(&pixels[0]);
	}
}
