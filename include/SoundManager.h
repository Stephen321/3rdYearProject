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
	void playSound(const std::string & name = "", bool looped = false, float volume = 1.f, sf::Vector2f position = sf::Vector2f(), float pitch = 1.f);
	void stopSound(const std::string & name); //called on looped sounds
	void setListener(Player * _player);
	void update();
private:
	SoundManager();
	static std::weak_ptr<SoundManager> m_instance;
	friend std::weak_ptr <SoundManager>;
	friend std::shared_ptr <SoundManager>;
	Player * player;
	FMOD::System * m_system;
	std::unordered_map<std::string, //key
					   std::pair<FMOD::Sound*, FMOD::Channel*>> //pair of sound and channel that sound is on if its looped (other wise this is 0)
					   m_sounds; //the map
	int SOUNDS_AMOUNT;
	void ERRCHECK(FMOD_RESULT result);
	FMOD::Reverb * reverb;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::CircleShape listenerCircle;
	sf::CircleShape reverbCircle;
	sf::CircleShape reverbCircle2;
};

#include "Player.h"

#endif