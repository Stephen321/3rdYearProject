#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <memory>
#include "Animation.hpp"
#include <unordered_map>
#include "fmod.hpp"

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
	//sounds
	FMOD::Sound * birdTweet1;
	FMOD::Sound * birdTweet2;
	FMOD::Sound * birdTweet3;
	FMOD::Sound * birdTweet4;
	FMOD::Sound * birdTweet5;
	FMOD::Sound * crowSound;
	FMOD::Sound * windAmbience;
	//textures
	sf::Texture rockTexture;

private:
	GameData();
	static std::weak_ptr<GameData> m_instance;
	friend std::weak_ptr <GameData>;
	friend std::shared_ptr <GameData>;
};
#endif