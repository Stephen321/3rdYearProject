#include "SoundManager.h"

std::weak_ptr<SoundManager> SoundManager::m_instance;

SoundManager::SoundManager(){
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_system);
	ERRCHECK(result);
	result = m_system->init(32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
	SOUNDS_AMOUNT = 0;
	//debug circle
	listenerCircle = sf::CircleShape(10);
	listenerCircle.setFillColor(sf::Color(101, 205, 95));
	listenerCircle.setOrigin(10.f, 10.f);
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

void SoundManager::loadSound(const std::string & filePath, const std::string & fileName, const std::string & name, bool stream){
	FMOD::Sound * sound;
	FMOD_RESULT result;
	if (stream)
		result = m_system->createStream((filePath + fileName).c_str(), FMOD_2D | FMOD_DEFAULT, 0, &sound);
	else
		result = m_system->createSound((filePath + fileName).c_str(), FMOD_3D, 0, &sound);
	ERRCHECK(result); 
	m_sounds[name] = std::pair<FMOD::Sound*, std::vector<FMOD::Channel*>>(sound, std::vector<FMOD::Channel*>());
	SOUNDS_AMOUNT++;
}

void SoundManager::play3DSound(const std::string & name, bool looped, sf::Vector2f position, float pitch){
	FMOD::Sound * sound = m_sounds.at(name).first;
	FMOD::Channel * channel;
	if (true){
		FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
		ERRCHECK(result);
		const FMOD_VECTOR pos = { position.x, position.y, 0 };
		const FMOD_VECTOR vel = {}; 
		result = channel->set3DAttributes(&pos, &vel);
		ERRCHECK(result);
		result = channel->setPaused(false);
		ERRCHECK(result);
		if (looped){
			result = channel->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
		}
		m_sounds.at(name).second.push_back(channel);
	}
}

void SoundManager::playSound(const std::string & name, bool looped, float volume, float pitch){
	FMOD::Sound * sound = m_sounds.at(name).first;
	FMOD::Channel * channel;
	if (true){
		FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
		ERRCHECK(result);
		result = channel->setVolume(volume);
		ERRCHECK(result);
		if (looped){
			result = channel->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
		}
		m_sounds.at(name).second.push_back(channel);
	}
}

void SoundManager::stopSound(const std::string & name){
	//FMOD::Sound * sound = m_sounds.at(name).first;
	//FMOD::Channel * channel = m_sounds.at(name).second;
	//if (channel != 0){
	//	FMOD_RESULT result = channel->stop();
	//	channel = 0;
	//	ERRCHECK(result);
	//	m_sounds.at(name).second = channel;
	//}
}

void SoundManager::removeEmptyChannels(){
	for (auto itr = m_sounds.begin(); itr != m_sounds.end(); ++itr){
		std::vector<FMOD::Channel*>& channels = (*itr).second.second;
		for (auto cItr = channels.begin(); cItr != channels.end(); ++cItr){
			if ((*cItr) == 0)
				cItr = channels.erase(cItr);
		}
	}
}

void SoundManager::setListener(Player * _player){
	player = _player;
}

void SoundManager::update(){
	removeEmptyChannels();
	const FMOD_VECTOR pos = { player->getPosition().x, player->getPosition().y, 0 };
	const FMOD_VECTOR vel = { player->getVelocity().x, player->getVelocity().y, 0 };
	listenerCircle.setPosition(player->getPosition());
	FMOD_RESULT result = m_system->set3DListenerAttributes(0, &pos, &vel, 0, 0);
	ERRCHECK(result);
	m_system->update();
}

void SoundManager::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(listenerCircle);
}