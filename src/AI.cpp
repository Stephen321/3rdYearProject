#include "AI.h"

AI::AI(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale, float speed) :
Character(position, world, anims, playSpeed, scale, speed)
{}

void AI::update(sf::Time dt, sf::FloatRect viewBounds, sf::Vector2f playerPos){
	behaviour(playerPos);
	Character::update(dt, viewBounds);
}

void AI::behaviour(sf::Vector2f playerPos){
	sf::Vector2f vB = playerPos - m_position;
	float angle = std::atan2(vB.y, vB.x);
	float distance = std::sqrt(vB.x * vB.x + vB.y * vB.y);
	if (distance > 40)
		m_velocity = sf::Vector2f(std::cos(angle) * m_SPEED, std::sin(angle) * m_SPEED);
	else
		m_velocity = sf::Vector2f(0, 0);
	if (m_velocity.x > 0)
		m_animatedSprite.setScale(-m_scale, m_scale);
	else
		m_animatedSprite.setScale(m_scale, m_scale);
}