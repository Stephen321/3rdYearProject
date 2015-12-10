#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include "fmod.hpp"
#include "fmod_errors.h"

class SoundManager {
public:
	static std::shared_ptr<SoundManager> getInstance();
	void loadSound(const std::string & filePath, const std::string & name);
private:
	SoundManager();
	static std::weak_ptr<SoundManager> m_instance;
	friend std::weak_ptr <SoundManager>;
	friend std::shared_ptr <SoundManager>;

	FMOD::System * m_system;
	std::unordered_map<std::string, FMOD::Sound*> m_sounds;

	void ERRCHECK(FMOD_RESULT result);
};

#endif