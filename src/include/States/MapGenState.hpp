#ifndef STATES_MAPGENSTATE_HPP
#define STATES_MAPGENSTATE_HPP

#include "States/State.hpp"
#include "MapGen/MapGen.hpp"
#include "MapGen/SimpleMapGen.hpp"

#include "Graphics/Tilemap.hpp"
#include <imgui.h>

class MapGenState : public State {
	Tilemap &tilemap;
	
public:
	
	MapGenState(Tilemap &tilemap);
	virtual ~MapGenState();
	
	// returns generation time in seconds
	double generate(int width, int height, int seed);
	
	virtual void draw();
};

#endif
