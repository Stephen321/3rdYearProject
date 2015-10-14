#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics.hpp" 
class Player : sf::Drawable{
public:
	Player(sf::Vector2f position);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	float m_moveSpeed;
	sf::Vector2f m_position;
	sf::Vector2f m_direction;
	sf::Sprite m_sprite;
};
#endif