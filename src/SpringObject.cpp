#include "SpringObject.h"

SpringObject::SpringObject(sf::Sprite sprite, int xOffset, int yOffset){

	this->sprite = sprite;
	m_origin = sf::Vector2f(400, 450);
	m_restingPos = sf::Vector2f(m_origin.x + xOffset, m_origin.y + yOffset);

	m_position = sf::Vector2f(m_origin.x, m_origin.y);
	m_velocity = sf::Vector2f(0, 0);
	accel = sf::Vector2f(0, 0);
	m_dampingConst = 0.4f;
	m_springConst = 0.05f;
	m_mass = 0.009;
}

SpringObject::~SpringObject(){}

void SpringObject::update(float dt){
	sf::Vector2f v = m_position - m_restingPos;
	accel = -((m_springConst / m_mass)*v) - (m_dampingConst / m_mass)* m_velocity;

	m_velocity += accel*(dt * 10);
	m_position += (m_velocity*dt) + (0.5f * accel * (dt * dt * 10));	

	sprite.setPosition(m_position);
}

void SpringObject::draw(sf::RenderTarget &target, sf::RenderStates states) const{
	target.draw(sprite);
}