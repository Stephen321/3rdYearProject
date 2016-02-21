#include "TallGrass.h"

TallGrass::TallGrass(b2World& world, sf::Vector2f position) :
GameObject(world, position, ObjectType::TALLGRASS) {}

void TallGrass::update(sf::FloatRect viewBounds)
{
	m_visible = m_bounds.intersects(viewBounds);


}
void TallGrass::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (m_visible){
		target.draw(m_sprite);
		//target.draw(centreTest);
		//target.draw(boundsTest);
	}
}