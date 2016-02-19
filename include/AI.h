#ifndef AI_H
#define AI_H

#include "Character.h"

class Player;

class AI : public Character{
public:
	AI(b2World& world, Player* playerP, sf::Vector2f position, Pathfinder * pf);
	void update(sf::Time dt, sf::FloatRect viewBounds);
	void sensorEnd(Character*) override;
	void sensorStart(Character*) override;
private:
	void behaviour();
	float attackTimer;
	const int ATTACK_TIME = 1.2f;
	Character* target;
	Player* player;
	sf::Vector2i m_lastPlayerTileCoord;
	std::vector<sf::Vector2f> m_waypoints;
};
#include "Player.h"

#endif