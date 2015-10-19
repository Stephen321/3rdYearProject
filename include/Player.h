#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics.hpp" 
#include <fstream>
#include "rapidjson\document.h"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include <memory>
#include <unordered_map>
#include "GameData.h"

//using namespace rapidjson;

class Player : public sf::Drawable {
public:
	Player(sf::Vector2f position);
	sf::Vector2f getPosition() const;
	void update(sf::Time dt);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void checkInput();
	AnimatedSprite m_animatedSprite;
	const int m_SPEED = 50; //pixels per second
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	Animation* currentAnim;
	std::unordered_map<std::string, Animation> m_anims;
};
#endif