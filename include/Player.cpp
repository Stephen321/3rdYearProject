#include "Player.h"

Player::Player(sf::Vector2f position, sf::Texture& texture) : m_position(position) {
	m_sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	m_sprite.setTexture(texture);
	m_sprite.setScale(sf::Vector2f(0.3,0.3));
	m_sprite.setPosition(m_position);
}


void Player::update(float dt){
	checkInput();
	m_position += m_velocity * dt;
	m_sprite.setPosition(m_position);
}

void Player::checkInput(){
	float xPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
	float yPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
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

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(m_sprite);
}

sf::Vector2f Player::getPosition(){
	return m_position;
}