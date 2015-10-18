#include "Player.h"

Player::Player(sf::Vector2f position) : m_position(position) {
	m_animatedSprite = AnimatedSprite(sf::seconds(0.0333f), false, true);
	m_animatedSprite.setScale(0.2f, 0.2f);
	//m_animatedSprite.setOrigin(rect.width / 2.f, rect.height / 2.f);
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	m_anims = ptr->playerAnims;
	currentAnim = &m_anims.at("kick");
}

void Player::update(sf::Time dt){
	checkInput();
	m_position += m_velocity * dt.asSeconds();
	m_animatedSprite.setPosition(m_position);
	m_animatedSprite.play(*currentAnim);
	m_animatedSprite.update(dt);
	m_animatedSprite.setOrigin(m_animatedSprite.getLocalBounds().width / 2.f, m_animatedSprite.getLocalBounds().height / 2.f);//update origin
}

void Player::checkInput(){
	bool test = sf::Joystick::isConnected(0);
	if (sf::Joystick::isButtonPressed(0, 0))
		currentAnim = &m_anims.at("derp");
	if (sf::Joystick::isButtonPressed(0, 1))
		currentAnim = &m_anims.at("kick");
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
	target.draw(m_animatedSprite);

}

sf::Vector2f Player::getPosition() const{
	return m_position;
}