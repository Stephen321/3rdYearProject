#ifndef TREE_H
#define TREE_H

#include "GameObject.h"

class Tree : public GameObject{
public:
	Tree(b2World& world, sf::Vector2f position);
};
 
#endif