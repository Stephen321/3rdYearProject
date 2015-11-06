#include "AI.h"

AI::AI(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale) :
Character(position, world, anims, playSpeed, scale),
m_visible(false)
{}

void AI::behaviour(float dt){
	//int joystick = -1;
	//for (int i = 0; i < 6 && joystick == -1; i++){
	//	if (sf::Joystick::isConnected(i))
	//		joystick = i;
	//}
	//if (sf::Joystick::isButtonPressed(joystick, 0))
	//	currentAnim = &m_anims["derp"];
	//if (sf::Joystick::isButtonPressed(joystick, 1))
	//	currentAnim = &m_anims["kick"];
	//if (sf::Joystick::isButtonPressed(joystick, 2))
	//	currentAnim = &m_anims["win"];
	//float xPos = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::X);
	//float yPos = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
	//if ((xPos < 10 && xPos > -10) &&
	//	(yPos < 10 && yPos > -10)){
	//	m_velocity.x = 0;
	//	m_velocity.y = 0;
	//	return;
	//}
	//xPos = (xPos / 100) * m_SPEED;
	//yPos = (yPos / 100) * m_SPEED;

	//m_velocity = sf::Vector2f(xPos, yPos);
}



void AI::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	sf::FloatRect bounds;
	sf::View view = target.getView();
	bounds.left = view.getCenter().x - view.getSize().x / 2.f;
	bounds.top = view.getCenter().y - view.getSize().y / 2.f;
	bounds.width = view.getSize().x;
	bounds.height = view.getSize().y;
	m_visible = m_animatedSprite.getGlobalBounds().intersects(bounds);
	if (m_visible)
		Character::draw(target, states);
}