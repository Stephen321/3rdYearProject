#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Player;

class Enemy : public Character{
public:
	Enemy(b2World& world, Player* playerP, sf::Vector2f position, Pathfinder * pf);
	void sensorEnd(Character*) override;
	void sensorStart(Character*) override;

protected:
	virtual void behaviour() override;
	virtual void setTarget();
	virtual void getWaypoints();
	virtual void followPath();
	virtual void handleAttack();

	bool m_followPlayer;
	sf::Vector2f m_startPos;
	int m_visibiltyRange;
	int m_attackTime;
	Character* m_target;
	Player* player;
	sf::Vector2i m_lastTargetTileCoord;
	sf::Vector2i m_targetTileCoord;
	std::vector<sf::Vector2f> m_waypoints;
};
#include "Player.h"

#endif