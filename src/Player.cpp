#include "Player.h"
#include <iostream> //testing

Player::Player(b2World& world, sf::Vector2f position) :
Character(world, CharacterType::PLAYER, position),
comboAnim(nullptr){
}

void Player::update(sf::Time dt, sf::FloatRect viewBounds){
	behaviour();
	Character::update(dt, viewBounds);
}

void Player::behaviour(){
	int joystick = -1;
	for (int i = 0; i < 6 && joystick == -1; i++){
		if (sf::Joystick::isConnected(i))
			joystick = i;
	}
	if (m_currentEvent.type == sf::Event::JoystickButtonPressed && previousRelease && m_currentEvent.joystickButton.button == 0){
		if (m_attacking == false){
			currentAnim = &m_anims["punch"];
			if (Debug::soundEffects)
				sndMgr->playSound("punch", false);
			m_animatedSprite.play(*currentAnim);
			m_animatedSprite.setLooped(false);
			m_attacking = true;
			if (attackableEnemies.size() != 0)
				attackableEnemies[0]->takeDamage(50); 
		}
	}
	if (m_currentEvent.type == sf::Event::JoystickButtonPressed && previousRelease && m_currentEvent.joystickButton.button == 1){
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

	if (m_currentEvent.type == sf::Event::JoystickButtonPressed && previousRelease && m_currentEvent.joystickButton.button == 2){
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
			if (currentAnim == &m_anims["kick"] && m_animatedSprite.getFrame() < 7){
				comboAnim = &m_anims["flip"];
				frameToChange = 7;
			}
		}
	}
	if (m_attacking && comboed == false && comboAnim != nullptr && m_animatedSprite.getFrame() == frameToChange){
		m_animatedSprite.play(*comboAnim);
		comboed = true;
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
		comboAnim = nullptr;
		frameToChange = -1;
		comboed = false;
	}
	previousRelease = m_currentEvent.type == sf::Event::JoystickButtonPressed;
	if (Debug::displayInfo) std::cout << attackableEnemies.size() << std::endl;
	float xPos = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::X);
	float yPos = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
	if ((xPos < 10 && xPos > -10) &&
		(yPos < 10 && yPos > -10)){
		m_velocity.x = 0;
		m_velocity.y = 0;
		sndMgr->stopSound("walking_grass");
		return;
	}
	if (Debug::soundEffects)
		sndMgr->playSound("walking_grass", true);
	xPos = (xPos / 100) * m_speed;
	yPos = (yPos / 100) * m_speed;

	m_velocity = sf::Vector2f(xPos, yPos);
}

void Player::setEvent(sf::Event e){
	m_currentEvent = e;
}

sf::Vector2f Player::getVelocity(){
	return tmx::BoxToSfVec(m_body->GetLinearVelocity());
}

void Player::startContact(){
}

void Player::endContact(){

}

void Player::sensorEnd(Character* e){
	attackableEnemies.erase(std::find(attackableEnemies.begin(), attackableEnemies.end(), e));
}

void Player::sensorStart(Character* e){

	attackableEnemies.push_back(e);
}