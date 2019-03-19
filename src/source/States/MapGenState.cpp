#include "States/MapGenState.hpp"

MapGenState::MapGenState(Tilemap &tilemap):
	tilemap(tilemap)
{
	
}

MapGenState::~MapGenState() {
	
}

double MapGenState::generate(int w, int h, int seed) {
	tilemap.create(w, h);
	
	SimpleMapGenerator mgen(seed);
	
	double begin_s = glfwGetTime();
	mgen.generate(tilemap);
	double end_s = glfwGetTime();
	double dt = end_s - begin_s;
	//printf("Generating world with seed %lu took %g ms.\n", mgen.getSeed(), dt * 1000.0f);
	//tilemap.clearCache();
	
	return dt;
}

void MapGenState::draw() {
	glClearColor(0.7, 0.8, 0.65, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	static int map_size = 250;
	static double genTime_s = -1.0;
	static int map_seed = rand();
	
	if (ImGui::Begin("Generator Settings")) {
		ImGui::InputInt("Seed", &map_seed);
		
		ImGui::SliderInt("Size (in Tiles)", &map_size, 32, 512);
		if (genTime_s > 0.0)
			ImGui::Text("Generating took %.2f seconds.", genTime_s);
		
		if (ImGui::Button("Generate")) {
			genTime_s = generate(map_size, map_size, map_seed);
		}
		ImGui::SameLine();
		if (ImGui::Button("Enter")) {
			setFinished();
		}
	}
	ImGui::End();
}

