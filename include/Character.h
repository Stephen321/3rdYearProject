#ifndef CHARACTER_H
#define CHARACTER_H

#include <fstream>
#include "rapidjson\document.h"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include <memory>
#include <unordered_map>
#include "GameData.h"
#include "HealthBar.h"
#include "Box2D\Collision\Shapes\b2CircleShape.h"
#include "Debug.h"
#include "VisibleObject.h"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <queue>

class SoundManager;

class Character : public VisibleObject{

public:
	enum class CharacterType {
		AI, 
		POPOUT,
		PLAYER
	};

	void update(sf::Time dt, sf::FloatRect viewBounds);
	bool getVisible() const;
	virtual void startContact();
	virtual void endContact();
	void takeDamage(float damage);
	virtual void sensorEnd(Character*) = 0;
	virtual void sensorStart(Character*) = 0;
	void setVelocity(sf::Vector2f value);
	virtual sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f position);
	bool getAlive() const;
	void reset(sf::Vector2f resetPos);
	sf::Vector2i getTileCoord() const;
	sf::Vector2i getTileCoord(sf::Vector2f position) const;

protected:
	Character(b2World& world, CharacterType charType, const sf::Vector2f& position, Pathfinder * pf = 0);
	virtual void behaviour() = 0;
	void setUpBox2D(b2World& world, const b2Vec2& position, const GameData::CharInfo* info);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
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
	CharacterType m_charType;
	std::shared_ptr<SoundManager> sndMgr;
	float m_speed;//pixels per second	
	bool m_alive;
	Pathfinder * m_pathFinder;
	float m_timer;
	float dt;
};


#include "SoundManager.h"

#endif