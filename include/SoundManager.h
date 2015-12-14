#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include "fmod.hpp"
#include "fmod_errors.h"
#include "SFML\Graphics.hpp"

class Player;

class SoundManager {
public:
	static std::shared_ptr<SoundManager> getInstance();
	void loadSound(const std::string & filePath, const std::string & fileName, const std::string & name);
	void playSound(const std::string & name = "", bool looped = false, float volume = 1.f, float pitch = 1.f); 
	void play3DSound(const std::string & name = "", bool looped = false, sf::Vector2f position = sf::Vector2f(), float volume = 1.f, float pitch = 1.f);
	void stopSound(const std::string & name);
	void setListener(Player * _player);
	void update();
private:
	SoundManager();
	static std::weak_ptr<SoundManager> m_instance;
	friend std::weak_ptr <SoundManager>;
	friend std::shared_ptr <SoundManager>;
	Player * player;
	FMOD::System * m_system;
	//Player * player;
	std::unordered_map<std::string, std::pair<FMOD::Sound*, FMOD::Channel*>> m_sounds;
	int SOUNDS_AMOUNT;
	void ERRCHECK(FMOD_RESULT result);
};
#include "Player.h"

#endif