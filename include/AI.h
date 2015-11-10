#ifndef AI_H
#define AI_H

#include "Character.h"

class AI : public Character{
public:
	AI(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale, float speed);
	void update(sf::Time dt, sf::FloatRect viewBounds, sf::Vector2f playerPos);
	void sensorEnd(Character*) override;
	void sensorStart(Character*) override;
private:
	void behaviour(sf::Vector2f playerPos);
	float attackTimer;
	const int ATTACK_TIME = 1.2f;
	Character* target;
};
#endif