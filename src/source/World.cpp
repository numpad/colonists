#include "World.hpp"

World::World(int w, int h)
	: groundLayer(w, h)
{
	groundLayer.setTileset(assets.loadTexture("res/tileset.png"), 10, 10, 96);
}

glm::vec2 World::getCenterPoint() {
	return glm::vec2(groundLayer.getWidth() * groundLayer.getTilesize() * 0.5f,
		groundLayer.getHeight() * groundLayer.getTilesize() * 0.5f);
}

void World::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform *= this->getTransform();
	
	target.draw(this->groundLayer, states);
}
