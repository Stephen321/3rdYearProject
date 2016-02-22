#include "SpringObject.h"

SpringObject::SpringObject(sf::Sprite sprite){

	this->sprite = sprite;
	m_origin = sf::Vector2f(150, 300);
	m_restingPos = sf::Vector2f(m_origin.x - 50, m_origin.y - 200);

	m_position = sf::Vector2f(m_origin.x, m_origin.y);
	m_velocity = sf::Vector2f(0, 0);
	accel = sf::Vector2f(0, 0);
	m_dampingConst = 0.4f;
	m_springConst = 0.05f;
	m_mass = 100;
}

SpringObject::~SpringObject(){}

void SpringObject::update(){
	sf::Vector2f v = m_position - m_restingPos;
	accel = -((m_springConst / m_mass)*v) - (m_dampingConst / m_mass)* m_velocity;

	//m_velocity += accel*dt;
	//m_position += (m_velocity*dt) + (0.5f * accel * (dt * dt));


	m_velocity += accel;
	m_position += m_velocity;
}