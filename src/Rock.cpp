#include "Rock.h"

Rock::Rock(b2World& world, sf::Vector2f position) :
GameObject(world, position, CollisionFilters::ROCK, ObjectType::ROCK) {}