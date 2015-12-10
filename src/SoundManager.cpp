#include "SoundManager.h"

std::weak_ptr<SoundManager> SoundManager::m_instance;

SoundManager::SoundManager(){}

std::shared_ptr<SoundManager> SoundManager::getInstance(){
	std::shared_ptr<SoundManager> ptr = m_instance.lock();
	if (!ptr){
		ptr = std::make_shared<SoundManager>(SoundManager());
		m_instance = std::weak_ptr<SoundManager>(ptr);
	}
	return ptr;
}