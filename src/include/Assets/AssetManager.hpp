#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

class AssetManager {
	std::unordered_map<std::string, sf::Texture *> textures;
	
public:
	
	~AssetManager();
	
	sf::Texture *loadTexture(std::string path);
};

#endif
