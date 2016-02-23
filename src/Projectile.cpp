#include "Projectile.h"

Projectile::Projectile(sf::Sprite sprite, float damage, const sf::Vector2f& startPos, Player* p) :
m_sprite(sprite),
m_damage(damage),
m_position(startPos),
player(p),
m_timer(0),
m_alive(true){
	m_offset = sf::Vector2f(0, -75);
	sf::Vector2f vB = (player->getPosition() + m_offset) - m_position;
	float distance = std::sqrt(vB.x * vB.x + vB.y * vB.y);
	m_velocity = (vB / distance) * SPEED;
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height / 2.f);
}

void Projectile::update(float dt){
	isColliding();
	if (m_alive){
		m_timer += dt;
		m_position += m_velocity * dt;
		m_sprite.setPosition(m_position);
		if (m_timer > MAX_TRAVEL_TIME)
			m_alive = false;
	}
}

void Projectile::isColliding(){
	if (m_alive){
		sf::Vector2f vB = (player->getPosition() + m_offset) - m_position;
		float distanceSquared = vB.x * vB.x + vB.y * vB.y;
		if (distanceSquared < (COLLISION_RANGE * COLLISION_RANGE)){
			player->takeDamage(m_damage);
			m_alive = false;
		}
		else{
			m_alive = true;
		}
	}
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const{
	if (m_alive) target.draw(m_sprite);
}

Projectile & Projectile::operator=(const Projectile & rhs)
{
	if (this != &rhs){
		player = rhs.player;
		m_alive = rhs.m_alive;
	}
	return *this;
}