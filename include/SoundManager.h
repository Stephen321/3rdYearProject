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

class SoundManager : public sf::Drawable {
public:
	static std::shared_ptr<SoundManager> getInstance();
	void loadSound(const std::string & filePath = "", const std::string & fileName = "", const std::string & name = "", bool stream = false);
	void playSound(const std::string & name = "", bool looped = false, float volume = 1.f, float pitch = 1.f); 
	void play3DSound(const std::string & name = "", bool looped = false, sf::Vector2f position = sf::Vector2f(), float pitch = 1.f);
	void stopSound(const std::string & name);
	void setListener(Player * _player);
	void update();
private:
	SoundManager();
	void removeEmptyChannels();
	static std::weak_ptr<SoundManager> m_instance;
	friend std::weak_ptr <SoundManager>;
	friend std::shared_ptr <SoundManager>;
	Player * player;
	FMOD::System * m_system;
	std::unordered_map<std::string, //key
					   std::pair<FMOD::Sound*, std::vector<FMOD::Channel*>>> //pair of sound and vector of all channels playing that sound
					   m_sounds; //the map
	int SOUNDS_AMOUNT;
	void ERRCHECK(FMOD_RESULT result);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::CircleShape listenerCircle;
};

#include "Player.h"

#endif