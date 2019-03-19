#include "States/MapGenState.hpp"

MapGenState::MapGenState(Tilemap &tilemap):
	tilemap(tilemap)
{
	mapGens.push_back("Simple");
	mapGens.push_back("Perlin Noise");
}

MapGenState::~MapGenState() {
	
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
	
	static int map_size = 250;
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
		}
		
		ImGui::SliderInt("Size (in Tiles)", &map_size, 32, 512);
		if (genTime_s > 0.0)
			ImGui::Text("Generating took %.2f seconds.", genTime_s);
		
		if (ImGui::Button("Generate")) {
			genTime_s = generate(map_size, map_size);
		}
		ImGui::SameLine();
		if (ImGui::Button("Enter")) {
			setFinished();
		}
	}
	ImGui::End();
	
	if (currentMapGen == 1) {
		if (ImGui::Begin("Perlin noise settings")) {
			ImGui::SliderInt("Octaves", &genOctaves, 1, 16);
			ImGui::SliderFloat("Redist", &genRedistExp, 0.0f, 10.0f);
			ImGui::SliderFloat("Frequency", &genFreq, 0.00001f, 10.0f);
		}
		ImGui::End();
		
		if (ImGui::Begin("Elevation map preview")) {
			ImGui::Image(0, ImVec2(256, 256));
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
