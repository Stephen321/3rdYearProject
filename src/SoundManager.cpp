#include "SoundManager.h"

std::weak_ptr<SoundManager> SoundManager::m_instance;

SoundManager::SoundManager(){
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_system);
	ERRCHECK(result);
	result = m_system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
}

std::shared_ptr<SoundManager> SoundManager::getInstance(){
	std::shared_ptr<SoundManager> ptr = m_instance.lock();
	if (!ptr){
		ptr = std::make_shared<SoundManager>(SoundManager());
		m_instance = std::weak_ptr<SoundManager>(ptr);
	}
	return ptr;
}


void SoundManager::ERRCHECK(FMOD_RESULT result){
	if (result != FMOD_OK){
		std::cout << "FMOD ERROR! " << FMOD_ErrorString(result) << std::endl;
		//exit(-1);
	}
}

void SoundManager::loadSound(const std::string & filePath, const std::string & name){
	FMOD::Sound * sound;
	FMOD_RESULT result = m_system->createSound((filePath).c_str(), FMOD_3D, 0, &sound);
	ERRCHECK(result);
}