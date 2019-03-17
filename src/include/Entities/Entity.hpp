#ifndef ENTITIES_ENTITY_HPP
#define ENTITIES_ENTITY_HPP

#include <glm/glm.hpp>

class Entity {
	glm::vec2 *position;
	float lifepoints = 100.0f;
	
public:
	
	Entity(glm::vec2 *world_pos);
	
	float getLifepoints();
	void setLifepoints(float value);
	
};

#endif
