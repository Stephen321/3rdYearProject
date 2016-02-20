#include "Player.h"
#include <iostream> //testing

Player::Player(b2World& world, sf::Vector2f position) :
Character(world, CharacterType::PLAYER, position){
}

void Player::update(sf::Time dt, sf::FloatRect viewBounds){
	behaviour();
	Character::update(dt, viewBounds);
}

void Player::handleEvent(sf::Event e){
	m_joystick = -1;
	for (int i = 0; i < 6 && m_joystick == -1; i++){
		if (sf::Joystick::isConnected(i))
			m_joystick = i;
	}
	if (e.type == sf::Event::JoystickButtonPressed  && e.joystickButton.button == 0){
		if (m_attacking == false){
			currentAnim = &m_anims["punch"];
			sndMgr->playSound("punch", false);
			m_animatedSprite.play(*currentAnim);
			m_animatedSprite.setLooped(false);
			m_attacking = true;
			if (attackableEnemies.size() != 0)
				attackableEnemies[0]->takeDamage(50);
		}
	}
	if (e.type == sf::Event::JoystickButtonPressed  && e.joystickButton.button == 1){
		std::cout << "pressed 1" << '\n';
		if (m_attacking == false){
			currentAnim = &m_anims["kick"];
			m_animatedSprite.play(*currentAnim);
			m_animatedSprite.setLooped(false);
			m_attacking = true;
			if (attackableEnemies.size() != 0){
				for (Character* c : attackableEnemies)
					c->takeDamage(15);
			}
		}
	}

	if (e.type == sf::Event::JoystickButtonPressed  && e.joystickButton.button == 2){
		if (m_attacking == false) {
			currentAnim = &m_anims["flip"];
			m_animatedSprite.play(*currentAnim);
			m_animatedSprite.setLooped(false);
			m_attacking = true;
			if (attackableEnemies.size() != 0){
				for (Character* c : attackableEnemies)
					c->takeDamage(5);
			}
		}
		else{
			if (currentAnim == &m_anims["kick"] && m_animatedSprite.getFrame() < 10){
				Combo c;
				c.name = "flip";
				c.frameChange = 12;
				comboQ.push(c);
			}
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

	
	//combos
	if (m_attacking == true && comboQ.size() != 0 && m_animatedSprite.getFrame() == comboQ.front().frameChange){
		Animation* anim = &m_anims[comboQ.front().name];
		comboQ.pop();
		m_animatedSprite.play(*anim);
		m_animatedSprite.setLooped(false);
		if (attackableEnemies.size() != 0){
			for (Character* c : attackableEnemies)
				c->takeDamage(15);
		}
	}
	if (m_animatedSprite.isPlaying() == false){
		currentAnim = &m_anims["idle"];
		m_animatedSprite.play(*currentAnim);
		m_animatedSprite.setLooped(true);
		m_attacking = false;
	}
	/*float xPos = 0;
	float yPos = 0;
	if (m_currentEvent.type == sf::Event::JoystickMoved)
	{
		if (m_currentEvent.joystickMove.axis == sf::Joystick::X)
			xPos = m_currentEvent.joystickMove.position;
		if (m_currentEvent.joystickMove.axis == sf::Joystick::Y)
			yPos = m_currentEvent.joystickMove.position;
	}
	std::cout << xPos << ", " << yPos << std::endl;*/
	if (Debug::displayInfo) std::cout << attackableEnemies.size() << std::endl;
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

sf::Vector2f Player::getVelocity(){
	return BoxToSfVec(m_body->GetLinearVelocity());
}

void Player::startContact(){
}

void Player::endContact(){

}

void Player::sensorEnd(Character* e){
	if (attackableEnemies.size() != 0)
		attackableEnemies.erase(std::find(attackableEnemies.begin(), attackableEnemies.end(), e));
}

void Player::sensorStart(Character* e){
	attackableEnemies.push_back(e);
}