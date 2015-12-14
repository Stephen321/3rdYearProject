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

void SoundManager::loadSound(const std::string & filePath, const std::string & fileName, const std::string & name){
	FMOD::Sound * sound;
	FMOD_RESULT result = m_system->createSound((filePath + fileName).c_str(), FMOD_3D, 0, &sound);
	ERRCHECK(result); 
	m_sounds[name] = std::pair<FMOD::Sound*, FMOD::Channel*>(sound, 0);
	SOUNDS_AMOUNT++;
}

void SoundManager::play3DSound(const std::string & name, bool looped, sf::Vector2f position, float volume, float pitch){
	FMOD::Sound * sound = m_sounds.at(name).first;
	FMOD::Channel * channel = m_sounds.at(name).second;
	bool isPlaying;
	FMOD_RESULT result = channel->isPlaying(&isPlaying);
	ERRCHECK(result);
	if (!isPlaying){
		result = m_system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
		ERRCHECK(result);
		result = channel->setPaused(false);
		ERRCHECK(result);
		result = channel->setVolume(volume);
		ERRCHECK(result);
		if (looped){
			const FMOD_VECTOR pos = {position.x, position.y, 0};
			const FMOD_VECTOR vel = {};
			result = channel->set3DAttributes(&pos, &vel);
			result = channel->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
		}
		m_sounds.at(name).second = channel;
	}
}

void SoundManager::playSound(const std::string & name, bool looped, float volume, float pitch){
	FMOD::Sound * sound = m_sounds.at(name).first;
	FMOD::Channel * channel = m_sounds.at(name).second;
	bool isPlaying;
	FMOD_RESULT result = channel->isPlaying(&isPlaying);
	ERRCHECK(result);
	if (!isPlaying){
		result = m_system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
		ERRCHECK(result);
		result = channel->setVolume(volume);
		ERRCHECK(result);
		if (looped){
			result = channel->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
		}
		m_sounds.at(name).second = channel;
	}
}

void SoundManager::stopSound(const std::string & name){
	FMOD::Sound * sound = m_sounds.at(name).first;
	FMOD::Channel * channel = m_sounds.at(name).second;
	if (channel != 0){
		bool isPlaying;
		FMOD_RESULT result = channel->isPlaying(&isPlaying);
		ERRCHECK(result);
		if (isPlaying){
			result = channel->stop();
			ERRCHECK(result);
		}
	}
}

void SoundManager::setListener(Player * _player){
	player = _player;
}

void SoundManager::update(){
	const FMOD_VECTOR pos = { player->getPosition().x, player->getPosition().y, 0 };
	const FMOD_VECTOR vel = { player->getVelocity().x, player->getVelocity().y, 0 };
	FMOD_RESULT result = m_system->set3DListenerAttributes(0, &pos, &vel, 0, 0);
	ERRCHECK(result);
	m_system->update();
}