#include "Assets/AssetManager.hpp"

AssetManager::~AssetManager() {
	for (auto kv : textures) {
		delete kv.second;
	}
}

sf::Texture *AssetManager::loadTexture(std::string path) {
	// check if already loaded
	auto it = textures.find(path);
	if (it != textures.end()) {
		// already loaded
		return it->second;
	}
	
	sf::Texture *result = new sf::Texture();
	if (!result->loadFromFile(path)) {
		delete result;
		result = nullptr;
	}
	
	textures[path] = result;
	
	return result;
}

