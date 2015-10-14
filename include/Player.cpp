#include "Player.h"

Player::Player(sf::Vector2f position) : m_position(position){

}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(m_sprite);
}