#include "Player.h"
#include <iostream> //testing

Player::Player(b2World& world, sf::Vector2f position) :
Character(world, CharacterType::PLAYER, position),
m_actionToPlay(false),
m_comboString(""){
	m_actions[0] = Action(0, 11, 0, 8, 2, false, "swipe"); //A
	m_actions[1] = Action(0, 12, 1, 10, 3, false, "kick"); //B
	m_actions[2] = Action(0, 10, 2, 5, 2, false, "jumpSplits"); //X
	m_actions[3] = Action(3, 11, 3, 8, 4, false, "headbutt"); //Y
	m_actions[4] = Action(3, 11, 4, 12, 4, false, "kickFlip"); //kickFlip
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
			bool addCombo = false;
			if (!m_currentActions.empty() && m_actionToPlay == false){
				if (m_animatedSprite.getFrame() > m_currentActions.front()->getMinFrame() &&
					m_animatedSprite.getFrame() < m_currentActions.front()->getMaxFrame()){
					m_currentActions.push(&m_actions[buttonId]);
					m_actionToPlay = true;
					addCombo = true;
				}
			}
			else if (m_actionToPlay == false){
				m_currentActions.push(&m_actions[buttonId]);
				addCombo = true;
				currentAnim = &m_anims[m_currentActions.back()->getAnimName()];
				m_animatedSprite.play(*currentAnim);
				m_animatedSprite.setLooped(false);
				applyDamage();
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
			}
		}
	}
}

void Player::applyDamage(float comboMod){
	float damage = m_currentActions.back()->getDamage();
	int mod = m_currentActions.back()->getModifier();
	damage += ((rand() / (float)RAND_MAX) * (2 * mod)) - mod;
	float diminisher = (m_comboString.length() < 2) ? 1.f : 1.f - (m_comboString.length() / 10.f);
	damage *= diminisher * comboMod;
	
	std::cout << "damage by " <<  m_comboString << " is " << damage << "!!" << '\n';
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
	if (m_actionToPlay && m_animatedSprite.getFrame() == m_currentActions.front()->getMaxFrame()){
		m_currentActions.pop();
		m_animatedSprite.play(m_anims[m_currentActions.front()->getAnimName()]);
		m_animatedSprite.setLooped(false);
		m_actionToPlay = false;
		applyDamage();
	}

	//if all animations have stopped playing then reset to idle 
	if (m_animatedSprite.isPlaying() == false || (currentAnim == &m_anims["run"]  && m_velocity == sf::Vector2f())){
		m_speed = max_speed;
		currentAnim = &m_anims["idle"];
		m_animatedSprite.play(*currentAnim);
		m_animatedSprite.setLooped(false);
		m_actionToPlay = false;
		comboFinished();
	}
	else if (currentAnim != &m_anims["run"]){
		m_speed = 0;
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
	if (m_speed > 0 && currentAnim != &m_anims["run"]){
		currentAnim = &m_anims["run"];
		m_animatedSprite.play(*currentAnim);
		m_animatedSprite.setLooped(false);
	}

	m_velocity = sf::Vector2f(xPos, yPos);
}

void Player::comboFinished(){
	if (m_comboString == "BX"){
		std::cout << "BX combo triggered" << '\n';
		m_currentActions.push(&m_actions[4]);
		currentAnim = &m_anims[m_currentActions.back()->getAnimName()];
		m_animatedSprite.play(*currentAnim);
		m_animatedSprite.setLooped(false);
		applyDamage(2.f);
	}
	m_currentActions.swap(std::queue<Action*>()); //empty the queue
	m_comboString = "";
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