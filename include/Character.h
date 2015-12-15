#ifndef CHARACTER_H
#define CHARACTER_H

#include <fstream>
#include "rapidjson\document.h"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include <memory>
#include <unordered_map>
#include "GameData.h"
#include "tmx2box2d.h"
#include "HealthBar.h"
#include "Box2D\Collision\Shapes\b2CircleShape.h"
#include "Debug.h"
#include "CollisionFilters.h"
#include "VisibleObject.h"

class SoundManager;

class Character : public VisibleObject{

public:
	enum class CharacterType {
		AI, 
		PLAYER
	};

	virtual void update(sf::Time dt, sf::FloatRect viewBounds);
	bool getVisible() const;
	virtual void startContact();
	virtual void endContact();
	void takeDamage(float damage);
	virtual void sensorEnd(Character*) = 0;
	virtual void sensorStart(Character*) = 0;
	void setVelocity(sf::Vector2f value);

protected:
	Character(sf::Vector2f position, b2World& world, CharacterType charType);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	AnimatedSprite m_animatedSprite;
	sf::Vector2f m_velocity;
	Animation* currentAnim;
	std::unordered_map<std::string, Animation> m_anims;
	b2Body* m_body;
	sf::CircleShape c;
	sf::CircleShape sensorCircle;
	sf::Vector2f m_spriteOffset;
	bool m_visible;
	float m_scale;
	HealthBar m_health;
	bool m_attacking;
	CharacterType m_charType;
	std::shared_ptr<SoundManager> sndMgr;
	float m_speed;//pixels per second	
};


#include "SoundManager.h"

#endif