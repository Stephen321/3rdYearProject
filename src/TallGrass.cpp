#include "TallGrass.h"

TallGrass::TallGrass(b2World& world, sf::Vector2f position) :
GameObject(world, position, ObjectType::TALLGRASS) {}

void TallGrass::update(sf::FloatRect viewBounds)
{
	m_visible = m_bounds.intersects(viewBounds);


}