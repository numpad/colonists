#include "Entities/Entity.hpp"

Entity::Entity(glm::vec2 *world_pos):
	position(world_pos)
{
	
}

float Entity::getLifepoints() {
	return lifepoints;
}

void Entity::setLifepoints(float value) {
	lifepoints = value;
}
