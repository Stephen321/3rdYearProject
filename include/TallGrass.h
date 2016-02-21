#ifndef TALLGRASS_H
#define TALLGRASS_H

#include "GameObject.h"

class TallGrass : public GameObject{
public:
	TallGrass(b2World& world, sf::Vector2f position);
	void update(sf::FloatRect viewBounds) override;
};
 
#endif