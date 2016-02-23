#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "SoundManager.h"
#include "Action.h"

class Player : public Character {
public:
	Player(b2World& world, sf::Vector2f position = sf::Vector2f(1,1));
	void handleEvent(sf::Event e);
	sf::Vector2f getVelocity();
	void sensorEnd(Character*) override;
	void sensorStart(Character*) override;
//protected:
private:
	void behaviour() override;
	void applyDamage(float comboMod = 1.f);
	void comboFinished();
	Action m_actions[5];
	std::queue<Action*> m_currentActions;
	std::string m_comboString;
	bool m_actionToPlay;
	bool m_comboTriggered;


	std::vector<Character*> attackableEnemies;
	int m_joystick;
};
#endif