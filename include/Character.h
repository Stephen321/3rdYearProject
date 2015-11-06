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
#include "Box2D\Collision\Shapes\b2CircleShape.h"

class Character : public sf::Drawable{

public:
	Character(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale);
	sf::Vector2f getPosition() const;
	void update(sf::Time dt);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void behaviour(float);
	AnimatedSprite m_animatedSprite;
	const int m_SPEED = 50; //pixels per second
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	Animation* currentAnim;
	std::unordered_map<std::string, Animation> m_anims;
	b2World& world;
	b2Body* m_body;
	sf::CircleShape c;
	sf::Vector2f m_spriteOffset;
	
};

#endif