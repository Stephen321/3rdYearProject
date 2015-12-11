#include "SoundManager.h"

std::weak_ptr<SoundManager> SoundManager::m_instance;

SoundManager::SoundManager(){
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_system);
	ERRCHECK(result);
	result = m_system->init(32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
	SOUNDS_AMOUNT = 0;
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

void SoundManager::loadSound(const std::string & filePath, const std::string & fileName){
	FMOD::Sound * sound;
	FMOD_RESULT result = m_system->createSound((filePath + fileName).c_str(), FMOD_DEFAULT, 0, &sound);
	ERRCHECK(result);
	m_sounds[fileName] = std::pair<FMOD::Sound*, FMOD::Channel*>(sound, 0);
	SOUNDS_AMOUNT++;
}

void SoundManager::playSound(const std::string & name, bool looped, float pitch){
	FMOD::Sound * sound = m_sounds.at(name).first;
	FMOD::Channel * channel = m_sounds.at(name).second;
	m_system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

void SoundManager::stopSound(const std::string & name){

}