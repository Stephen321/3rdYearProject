#include "Player.h"

Player::Player(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale, float speed) :
Character(position, world, anims, playSpeed, scale, speed){
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
	if (sf::Joystick::isButtonPressed(joystick, 0))
		currentAnim = &m_anims["derp"];
	if (sf::Joystick::isButtonPressed(joystick, 1))
		currentAnim = &m_anims["kick"];
	if (sf::Joystick::isButtonPressed(joystick, 2))
		currentAnim = &m_anims["win"];
	float xPos = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::X);
	float yPos = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
	if ((xPos < 10 && xPos > -10) &&
		(yPos < 10 && yPos > -10)){
		m_velocity.x = 0;
		m_velocity.y = 0;
		return;
	}
	xPos = (xPos / 100) * m_SPEED;
	yPos = (yPos / 100) * m_SPEED;

	m_velocity = sf::Vector2f(xPos, yPos);
}