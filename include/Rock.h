#ifndef ROCK_H
#define ROCK_H

#include "Object.h"

class Rock : public Object{
public:
	Rock(b2World& world, sf::Vector2f position);
};

#endif