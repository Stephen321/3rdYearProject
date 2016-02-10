#ifndef CHARACTER_H
#define CHARACTER_H

#include <fstream>
#include "rapidjson\document.h"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include <memory>
#include <unordered_map>
#include "GameData.h"
#include "tmx\tmx2box2d.h"
#include "HealthBar.h"
#include "Box2D\Collision\Shapes\b2CircleShape.h"
#include "Debug.h"
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
	virtual sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);
	bool getAlive() const;
	void reset(sf::Vector2f resetPos);

protected:
	Character(b2World& world, CharacterType charType, sf::Vector2f position);
	void setUpBox2D(b2World& world, b2Vec2 position, CollisionFilters filterCategory, CollisionFilters filterMask);
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
	bool m_alive;
};


#include "SoundManager.h"

#endif