#ifndef ENTITIES_ENTITY_HPP
#define ENTITIES_ENTITY_HPP

#include <string>
#include <glm/glm.hpp>

class Entity {
	glm::vec2 *position;
	float lifepoints = 100.0f;
	
	std::string name = "entity";
	
public:
	
	Entity(glm::vec2 *world_pos);
	
	float getLifepoints();
	void setLifepoints(float value);
	
	std::string getName();
	void setName(std::string name);
	
};

#endif
