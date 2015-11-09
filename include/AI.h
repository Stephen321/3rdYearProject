#ifndef AI_H
#define AI_H

#include "Character.h"

class AI : public Character{
public:
	AI(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale, float speed);
	void update(sf::Time dt, sf::FloatRect viewBounds, sf::Vector2f playerPos);
private:
	void behaviour(sf::Vector2f playerPos);
};
#endif