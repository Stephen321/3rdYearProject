#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include "fmod.hpp"
#include "fmod_errors.h"
#include "SFML\Graphics.hpp"

class SoundManager {
public:
	static std::shared_ptr<SoundManager> getInstance();
	void loadSound(const std::string & filePath, const std::string & name);
	void playSound(const std::string & name = "", bool looped = false, float pitch = 1.f);
	void stopSound(const std::string & name);
private:
	SoundManager();
	static std::weak_ptr<SoundManager> m_instance;
	friend std::weak_ptr <SoundManager>;
	friend std::shared_ptr <SoundManager>;

	FMOD::System * m_system;
	std::unordered_map<std::string, std::pair<FMOD::Sound*, FMOD::Channel*>> m_sounds;
	int SOUNDS_AMOUNT;
	void ERRCHECK(FMOD_RESULT result);
};

#endif