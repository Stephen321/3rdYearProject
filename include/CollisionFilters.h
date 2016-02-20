#ifndef COLLISIONFILTERS_H
#define COLLISIONFILTERS_H

enum class CollisionFilters : unsigned short {
	ENEMY = 0x0001,
	PLAYER = 0x0002,
	COLLIDABLE = 0x0004
};

#endif