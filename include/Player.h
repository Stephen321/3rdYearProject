#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics.hpp" 
#include <fstream>
#include "rapidjson\document.h"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"

//using namespace rapidjson;

class Player : public sf::Drawable {
public:
	Player(sf::Vector2f position, sf::Texture & spriteSheet);
	sf::Vector2f getPosition();
	void update(sf::Time dt);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void checkInput();
	void loadAnimations(sf::Texture & spriteSheet);
	AnimatedSprite m_animatedSprite;
	const int m_SPEED = 50; //pixels per second
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	Animation* currentAnim;
	Animation kickAnim;
};
#endif