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

	result = m_system->createReverb(&reverb);
	ERRCHECK(result);
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_BATHROOM;
	reverb->setProperties(&prop);
	FMOD_VECTOR pos = { 420, 540,0 };
	float mindist = 100.0f;
	float maxdist = 150.0f;
	reverb->set3DAttributes(&pos, mindist, maxdist);
	reverb->setActive(false);

	reverbCircle = sf::CircleShape(100);
	reverbCircle.setFillColor(sf::Color(0, 0, 100));
	reverbCircle.setOrigin(100.f, 100.f);
	reverbCircle.setPosition(420.f, 540.f);


	reverbCircle2 = sf::CircleShape(150);
	reverbCircle2.setFillColor(sf::Color(0, 0, 150));
	reverbCircle2.setOrigin(150.f, 150.f);
	reverbCircle2.setPosition(420.f, 540.f);
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
	else if (name != "punch" && name != "walking_grass")
		result = m_system->createSound((filePath + fileName).c_str(), FMOD_3D_LINEARROLLOFF, 0, &sound);
	else
		result = m_system->createSound((filePath + fileName).c_str(), FMOD_2D | FMOD_DEFAULT, 0, &sound);
	ERRCHECK(result); 
	m_sounds[name] = std::pair<FMOD::Sound*, FMOD::Channel*>(sound, 0);
	SOUNDS_AMOUNT++;
}

void SoundManager::playSound(const std::string & name, bool looped, float volume, sf::Vector2f position, float pitch){
	FMOD::Sound * sound = m_sounds.at(name).first;
	FMOD::Channel * channel;
	bool is3DSound = (position == sf::Vector2f()) ? false : true;
	if ((looped && m_sounds.at(name).second == 0) || looped == false){
		FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_FREE, sound, is3DSound, &channel);
		ERRCHECK(result);
		result = channel->setVolume(volume);
		ERRCHECK(result);
		if (is3DSound){
			const FMOD_VECTOR pos = { position.x, position.y, 0 };
			result = channel->set3DAttributes(&pos, 0);
			ERRCHECK(result);
			result = channel->setPaused(false);
			ERRCHECK(result);
			result = channel->set3DMinMaxDistance(0.1f, 600.f);
			ERRCHECK(result);
			result = channel->set3DDopplerLevel(0.8f);
			ERRCHECK(result);
		}
		if (looped){
			result = channel->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
			m_sounds.at(name).second = channel;
		}
	}
	else if (looped && is3DSound){
		const FMOD_VECTOR pos = { position.x, position.y, 0 };
		FMOD_RESULT result = m_sounds.at(name).second->set3DAttributes(&pos, 0);
		ERRCHECK(result);
	}
}

void SoundManager::stopSound(const std::string & name){
	FMOD::Channel * channel = m_sounds.at(name).second;
	if (channel != 0){
		FMOD_RESULT result = channel->stop();
		ERRCHECK(result);
		channel = 0;
		m_sounds.at(name).second = channel;
	}
}

void SoundManager::setListener(Player * _player){
	player = _player;
}

void SoundManager::update(){
	if (Debug::reverb){
		FMOD_REVERB_PROPERTIES prop1 = FMOD_PRESET_FOREST;
		FMOD_RESULT result = m_system->setReverbAmbientProperties(&prop1);
		ERRCHECK(result);
		reverb->setActive(true);
	}
	else{
		FMOD_REVERB_PROPERTIES prop1 = FMOD_PRESET_OFF;
		FMOD_RESULT result = m_system->setReverbAmbientProperties(&prop1);
		ERRCHECK(result);
		reverb->setActive(false);
	}
	const FMOD_VECTOR pos = { player->getPosition().x, player->getPosition().y, 0 };
	const FMOD_VECTOR vel = { player->getVelocity().x, player->getVelocity().y, 0 };
	listenerCircle.setPosition(player->getPosition());
	FMOD_RESULT result = m_system->set3DListenerAttributes(0, &pos, &vel, 0, 0);
	ERRCHECK(result);
	m_system->update();
}

void SoundManager::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (Debug::reverb){
		target.draw(reverbCircle2);
		target.draw(reverbCircle);
		target.draw(listenerCircle);
	}
}