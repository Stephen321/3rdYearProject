#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Player;

class Enemy : public Character{
public:
	Enemy(b2World& world, Player* playerP, sf::Vector2f position, Pathfinder * pf);
	void update(sf::Time dt, sf::FloatRect viewBounds) override;
	void sensorEnd(Character*) override;
	void sensorStart(Character*) override;
	virtual void behaviour() override;
	virtual void getWaypoints();
	virtual void followPath();
	virtual void handleAttack();

protected:
	float attackTimer;
	int ATTACK_TIME = 1.2f;
	Character* target;
	Player* player;
	sf::Vector2i m_lastPlayerTileCoord;
	std::vector<sf::Vector2f> m_waypoints;
};
#include "Player.h"

#endif