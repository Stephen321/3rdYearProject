#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character {
public:
	Player(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale, float speed);
	virtual void update(sf::Time dt, sf::FloatRect viewBounds) override;
//protected:
private:
	void behaviour();

};
#endif