#include "Bush.h"

Bush::Bush(b2World& world, sf::Vector2f position) :
GameObject(world, position, CollisionFilters::COLLIDABLE, ObjectType::BUSH) {}