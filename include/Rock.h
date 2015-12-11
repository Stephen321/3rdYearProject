#ifndef ROCK_H
#define ROCK_H

#include "GameObject.h"

class Rock : public GameObject{
public:
	Rock(b2World& world, sf::Vector2f position);
};
 
#endif