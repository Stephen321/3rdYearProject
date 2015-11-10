#ifndef CHARACTER_H
#define CHARACTER_H

#include "SFML/Graphics.hpp" 
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


class Character : public sf::Drawable{

public:
	enum collisionFilters {
		AI = 0x0001,
		PLAYER = 0x0002,
	};

	sf::Vector2f getPosition() const;
	virtual void update(sf::Time dt, sf::FloatRect viewBounds);
	bool getVisible() const;
	virtual void startContact();
	virtual void endContact();
	void takeDamage(float damage);
	virtual void sensorEnd(Character*) = 0;
	virtual void sensorStart(Character*) = 0;
	void setVelocity(sf::Vector2f value);

protected:
	Character(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale, float speed, float maxHealth, Character::collisionFilters filter);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	AnimatedSprite m_animatedSprite;
	const int SPEED; //pixels per second
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	Animation* currentAnim;
	std::unordered_map<std::string, Animation> m_anims;
	b2World& world;
	b2Body* m_body;
	sf::CircleShape c;
	sf::CircleShape sensorCircle;
	sf::Vector2f m_spriteOffset;
	bool m_visible;
	float m_scale;
	HealthBar m_health;
	const int MAX_HEALTH;
	collisionFilters m_filter;
	bool m_attacking;
	
};

#endif