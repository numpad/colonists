#ifndef STATES_MAPGENSTATE_HPP
#define STATES_MAPGENSTATE_HPP

#include "States/State.hpp"
#include "MapGen/MapGen.hpp"
#include "MapGen/SimpleMapGen.hpp"
#include "MapGen/PerlinMapGen.hpp"

#include "Graphics/Tilemap.hpp"
#include <imgui.h>

class MapGenState : public State {
	Tilemap &tilemap;
	
	int map_seed = rand();
	
	std::vector<char *> mapGens;
	int currentMapGen = 1;
	MapGenerator *getMapGen();
	
	int genOctaves = 1;
	float genRedistExp = 1.0f;
	float genFreq = 1.0f;
	
public:
	
	MapGenState(Tilemap &tilemap);
	virtual ~MapGenState();
	
	// returns generation time in seconds
	double generate(int width, int height);
	
	virtual void draw();
};

#endif
