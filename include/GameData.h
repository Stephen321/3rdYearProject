#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <memory>
#include "Animation.hpp"
#include <map>

class GameData {
public:
	static std::shared_ptr<GameData> getInstance();
	std::vector<std::shared_ptr<sf::Texture>> playerTextures;//have to be kept in memory otherwise white square
	std::map<std::string, Animation> playerAnims; //like a dictonary

private:
	GameData();
	static std::weak_ptr<GameData> m_instance;
	friend std::weak_ptr < GameData >;
	friend std::shared_ptr < GameData >;
};
#endif