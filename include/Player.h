#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character {
public:
	Player(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale, float speed);
	virtual void update(sf::Time dt, sf::FloatRect viewBounds) override;
	virtual void startContact() override;
	virtual void endContact() override;
	void sensorEnd(Character*) override;
	void sensorStart(Character*) override;	
	void setEvent(sf::Event e);
//protected:
private:
	void behaviour();
	sf::Vector2f startPos;
	std::vector<Character*> attackableEnemies;
	Animation* comboAnim;
	sf::Event m_currentEvent;
	bool comboed = false;
	int frameToChange = -1;
	bool previousRelease = false;
};
#endif