#include "Player.h"
#include <iostream> //testing

Player::Player(b2World& world, sf::Vector2f position) :
Character(world, CharacterType::PLAYER, position),
m_actionToPlay(false),
m_comboString(""){
	m_actions[0] = Action(0, 11, 0, 10, 2, false, "punch"); //A
	m_actions[1] = Action(0, 16, 1, 10, 2, false, "kick"); //B
	m_actions[2] = Action(0, 10, 2, 10, 2, false, "flip"); //X
	m_actions[3] = Action(3, 11, 3, 10, 2, false, "punch"); //Y
}

void Player::handleEvent(sf::Event e){
	m_joystick = -1;
	for (int i = 0; i < 6 && m_joystick == -1; i++){
		if (sf::Joystick::isConnected(i))
			m_joystick = i;
	}
	if (e.type == sf::Event::JoystickButtonPressed){
		int buttonId = e.joystickButton.button;
		if (buttonId < 4){
			bool addCombo = true;
			if (!m_currentActions.empty() && m_actionToPlay == false){
				if (m_animatedSprite.getFrame() > m_currentActions.front()->getMinFrame() &&
					m_animatedSprite.getFrame() < m_currentActions.front()->getMaxFrame()){
					m_currentActions.push(&m_actions[buttonId]);
					m_actionToPlay = true;
				}
			}
			else if (m_actionToPlay == false){
				m_currentActions.push(&m_actions[buttonId]);
				m_animatedSprite.play(m_anims[m_currentActions.back()->getAnimName()]);
				m_animatedSprite.setLooped(false);
				applyDamage();
			}
			else{
				addCombo = false;
			}
			if (addCombo){
				switch (buttonId)
				{
				case 0:
					m_comboString += 'A';
					break;
				case 1:
					m_comboString += 'B';
					break;
				case 2:
					m_comboString += 'X';
					break;
				case 3:
					m_comboString += 'Y';
					break;
				}
				std::cout << "combo string: " + m_comboString << '\n';
			}
		}
	}
}

void Player::applyDamage(){
	float damage = m_currentActions.back()->getDamage();
	int mod = m_currentActions.back()->getModifier();
	damage += (rand() % ((2 * mod) + 1)) - mod;
	float diminisher = (m_comboString.length() < 2) ? 1.f : 1.f - (m_comboString.length() / 10.f);
	damage *= diminisher;
	if (attackableEnemies.size() != 0){
		if (m_currentActions.back()->getMultiAttack()){
			for (Character* c : attackableEnemies)
				c->takeDamage(damage);
		}
		else{
			attackableEnemies[0]->takeDamage(damage);
		}
	}
}

void Player::behaviour(){
	for (const Character* e : attackableEnemies){
		if (e->getAlive() == false){
			attackableEnemies.erase(std::find(attackableEnemies.begin(), attackableEnemies.end(), e));
			break;
		}
	}
	if (m_actionToPlay && m_animatedSprite.getFrame() >= m_currentActions.front()->getMaxFrame()){
		m_currentActions.pop();
		m_animatedSprite.play(m_anims[m_currentActions.front()->getAnimName()]);
		m_animatedSprite.setLooped(false);
		m_actionToPlay = false;
		applyDamage();
	}

	//if all animations have stopped playing then reset to idle 
	if (m_animatedSprite.isPlaying() == false){
		currentAnim = &m_anims["idle"];
		m_animatedSprite.play(*currentAnim);
		m_animatedSprite.setLooped(true);
		m_currentActions.pop(); //pop last remainingg combo
		m_actionToPlay = false;
		comboFinished();
		m_comboString = "";
	}

	float xPos = sf::Joystick::getAxisPosition(m_joystick, sf::Joystick::Axis::X);
	float yPos = sf::Joystick::getAxisPosition(m_joystick, sf::Joystick::Axis::Y);
	if ((xPos < 10 && xPos > -10) &&
		(yPos < 10 && yPos > -10)){
		m_velocity.x = 0;
		m_velocity.y = 0;
		sndMgr->stopSound("walking_grass");
		return;
	}
	sndMgr->playSound("walking_grass", true);
	xPos = (xPos / 100) * m_speed;
	yPos = (yPos / 100) * m_speed;

	m_velocity = sf::Vector2f(xPos, yPos);
}

void Player::comboFinished(){
	if (m_comboString == XXYcombo){
		std::cout << "XXY combo triggered" << '\n';
	}
}

sf::Vector2f Player::getVelocity(){
	return BoxToSfVec(m_body->GetLinearVelocity());
}

void Player::sensorEnd(Character* e){
	if (attackableEnemies.size() != 0)
		attackableEnemies.erase(std::find(attackableEnemies.begin(), attackableEnemies.end(), e));
}

void Player::sensorStart(Character* e){
	attackableEnemies.push_back(e);
}