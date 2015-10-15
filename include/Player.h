#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics.hpp" 
class Player : public sf::Drawable {
public:
	Player(sf::Vector2f position, sf::Texture& texture);
	sf::Vector2f getPosition();
	void update(float dt);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void checkInput();
	const int m_SPEED = 50; //pixels per second
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Sprite m_sprite;
};
#endif