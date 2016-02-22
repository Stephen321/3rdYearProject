#ifndef SPRINGYPHYSICSOBJECT_H
#define SPRINGYPHYSICSOBJECT_H

#include "SFML\Graphics.hpp"
#include <memory>
#include "GameData.h"

class SpringyPhysicsObject{
public:
	SpringyPhysicsObject(sf::Sprite sprite);
	~SpringyPhysicsObject();
	void update();
private:
	sf::Sprite sprite;
	sf::Vector2f m_origin, m_position, m_velocity, accel, m_restingPos;
	float m_dampingConst, m_springConst, m_mass;
	
};

#endif