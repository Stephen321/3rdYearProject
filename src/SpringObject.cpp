#include "SpringObject.h"

SpringObject::SpringObject(sf::Sprite sprite){
	//-350 , -400
	this->sprite = sprite;
	m_origin = sf::Vector2f(400, 450);
	m_restingPos = sf::Vector2f(400, 100);// sf::Vector2f(m_origin.x + xOffset, m_origin.y + yOffset);

	m_position = m_origin;
	this->sprite.setPosition(m_position);

	m_velocity = sf::Vector2f(0, 0);
	accel = sf::Vector2f(0, 0);

	/*c1 = sf::CircleShape(5.f);
	c1.setOrigin(5, 5);
	c1.setOutlineThickness(1.5f);
	c1.setOutlineColor(sf::Color::Green);
	c1.setPosition(m_origin);

	c2 = c1;
	c2.setOutlineColor(sf::Color::Blue);
	c2.setPosition(m_restingPos);
	c3 = c1;
	c3.setOutlineColor(sf::Color::Red);
	c3.setPosition(m_position);*/
}

SpringObject::~SpringObject(){}

void SpringObject::setRestingPos(int offsetX, int offsetY)
{
	m_restingPos = sf::Vector2f(m_origin.x + offsetX, m_origin.y + offsetY);
}

void SpringObject::update(float dt){
	if (released)
	{
		sf::Vector2f v = m_position - m_restingPos;
		accel = ((-m_springConst * v) + (-m_dampingConst * m_velocity)) / m_mass;
		m_velocity += accel * dt;
		m_position += (m_velocity*dt) + (0.5f * accel * (dt * dt));
		sprite.setPosition(m_position);

		//c3.setPosition(m_position);
	}
}

void SpringObject::draw(sf::RenderTarget &target, sf::RenderStates states) const{
	if (released)
	{
		target.draw(sprite);
		//target.draw(c1);
		//target.draw(c2);
		//target.draw(c3);
	}
}

void SpringObject::setReleased()
{
	released = !released;
}