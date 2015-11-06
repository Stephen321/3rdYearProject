#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <memory>
#include "Animation.hpp"
#include <unordered_map>

class GameData {
public:
	static std::shared_ptr<GameData> getInstance();
	std::vector<std::shared_ptr<sf::Texture>> playerTextures;//have to be kept in memory otherwise white square
	std::unordered_map<std::string, Animation> playerAnims; //like a dictonary
	float playerPlaySpeed;
	float playerSpriteScale;

	//ai textures and animations
	std::vector<std::shared_ptr<sf::Texture>> aiTextures;
	std::unordered_map<std::string, Animation> aiAnims; 
	float aiPlaySpeed;
	float aiSpriteScale;

private:
	GameData();
	static std::weak_ptr<GameData> m_instance;
	friend std::weak_ptr < GameData >;
	friend std::shared_ptr < GameData >;
};
#endif