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
		
		ImGui::InputInt("Seed", &map_seed);
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
			if (ImGui::SliderInt("Octaves", &genOctaves, 1, 5))
				renderHeightmapPreview();
			
			if (ImGui::SliderFloat("Redist", &genRedistExp, 0.0f, 10.0f))
				renderHeightmapPreview();
			
			if (ImGui::SliderFloat("Frequency", &genFreq, 0.001f, 0.1f))
				renderHeightmapPreview();
		}
		ImGui::End();
		
		if (ImGui::Begin("Elevation map preview")) {
			heightmapPreview.active(0);
			ImGui::Image((ImTextureID)(intptr_t)(GLuint)heightmapPreview, ImVec2(map_size, map_size));
		}
		ImGui::End();
	}
}

void MapGenState::presetsWindow() {
	if (currentMapGen == 1) {
		if (ImGui::Begin("Load preset")) {
			if (ImGui::Button("Swamp")) {
				map_seed = rand();
				genOctaves = 4;
				genRedistExp = 0.9f;
				genFreq = 0.02f;
				renderHeightmapPreview();
			}
			if (ImGui::Button("Valley")) {
				map_seed = rand();
				genOctaves = 5;
				genRedistExp = 0.889f;
				genFreq = 0.005f;
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
	
	for (int y = 0; y < map_size; ++y) {
		for (int x = 0; x < map_size; ++x) {
			float e = gen->getElevationAt(x, y);
			GLubyte R = 0, G = 0, B = 0;
			if (e < 0.33f) { B = 1; e = 0.6f; } 
			else if (e < 0.66f) R = 1;
			else G = 1;
			
			pixels.at((x + y * map_size) * 4 + 0) = (e * 255) * R;
			pixels.at((x + y * map_size) * 4 + 1) = (e * 255) * G;
			pixels.at((x + y * map_size) * 4 + 2) = (e * 255) * B;
			pixels.at((x + y * map_size) * 4 + 3) = 255;
		}
	}
	
	if (heightmapPreview.getWidth() == map_size && heightmapPreview.getWidth() == map_size) {
		heightmapPreview.updateTextureFromMemory(&pixels[0]);
	} else {
		heightmapPreview.destroy();
		heightmapPreview.setTextureData(map_size, map_size, 4);
		heightmapPreview.loadTextureFromMemory(&pixels[0]);
	}
}
