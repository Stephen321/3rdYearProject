#ifndef POPOUT_H
#define POPOUT_H

#include "Projectile.h"
#include "Enemy.h"


class Popout : public Enemy{
public:
	Popout(b2World& world, Player* playerP, sf::Vector2f position, Pathfinder * pf);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void behaviour() override;
	void setTarget() override;
	sf::Vector2i getTileAround(const sf::Vector2i& playerCoord);
	void getWaypoints() override;
	void followPath() override;
	void handleAttack() override;
	void burrow();

	std::vector<Projectile> m_projectiles;
	float m_popOutTimer;
	const float POPUP_RANGE = 150.f;
	const float MAX_TIME = 5.f;
	bool m_inRange;
	const float DEGREES_TO_RADIANS = 3.14f / 180.f;
	sf::Texture* m_projectileTex;
};

#endif