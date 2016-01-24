#ifndef AI_H
#define AI_H

#include "Character.h"

class Player;

class AI : public Character{
public:
	AI(b2World& world, Player* playerP, sf::Vector2f position);
	void update(sf::Time dt, sf::FloatRect viewBounds);
	void sensorEnd(Character*) override;
	void sensorStart(Character*) override;
private:
	void behaviour();
	float attackTimer;
	const int ATTACK_TIME = 1.2f;
	Character* target;
	Player* player;
};
#include "Player.h"

#endif