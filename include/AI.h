#ifndef AI_H
#define AI_H

#include "Enemy.h"


class AI : public Enemy{
public:
	AI(b2World& world, Player* playerP, sf::Vector2f position, Pathfinder * pf);
	//void behaviour() override;
};

#endif