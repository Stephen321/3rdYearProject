#ifndef SPRINGOBJECT_H
#define SPRINGOBJECT_H

#include "SFML\Graphics.hpp"
#include <memory>
#include "GameData.h"

class SpringObject : public sf::Drawable
{
public:
	SpringObject(sf::Sprite sprite, int xOffset, int yOffset);
	~SpringObject();
	void update(float dt);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
	sf::Sprite sprite;
	sf::Vector2f m_origin, m_position, m_velocity, accel, m_restingPos;
	const float m_dampingConst = 0.4f;
	const float m_springConst = 0.9f;
	const float m_mass = 1.f;

	//test
	sf::CircleShape c1;
	sf::CircleShape c2;
	sf::CircleShape c3;
	
};

#endif