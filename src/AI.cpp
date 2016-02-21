#include "AI.h"

AI::AI(b2World& world, Player* playerP, sf::Vector2f position, Pathfinder * pf) :
Enemy(world,playerP, position, pf){}