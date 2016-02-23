#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Player.h"
#include "SFML\Graphics.hpp"

class Projectile : public sf::Drawable {
public:
	Projectile(sf::Sprite sprite, float damage, const sf::Vector2f& startPos, Player* p);
	void update(float dt);
	bool getAlive() const { return m_alive; }
	Projectile & operator=(const Projectile & rhs);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void isColliding();
	bool m_alive;
	float m_damage;
	Player* player;
	sf::Sprite m_sprite;
	sf::Vector2f m_position;
	sf::Vector2f m_offset;
	sf::Vector2f m_velocity;
	float m_timer;
	const float MAX_TRAVEL_TIME = 4.f;
	const float SPEED = 100.f;
	const float COLLISION_RANGE = 25.f;

};

#endif