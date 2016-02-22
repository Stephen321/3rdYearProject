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
	float m_dampingConst, m_springConst, m_mass;
	
};

#endif