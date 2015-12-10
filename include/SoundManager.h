#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <memory>
//#include <unordered_map>
#include "fmod.hpp"
#include "fmod_errors.h"

class SoundManager {
public:
	static std::shared_ptr<SoundManager> getInstance();
private:
	SoundManager();
	static std::weak_ptr<SoundManager> m_instance;
	friend std::weak_ptr <SoundManager>;
	friend std::shared_ptr <SoundManager>;


};

#endif