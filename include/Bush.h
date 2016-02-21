#ifndef BUSH_H
#define BUSH_H

#include "GameObject.h"

class Bush : public GameObject{
public:
	Bush(b2World& world, sf::Vector2f position);
};
 
#endif