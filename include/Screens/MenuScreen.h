#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <iostream>
#include <memory>
#include "Screen.hpp"
#include "GameData.h"

#include <SFML/Graphics.hpp>

class MenuScreen : public Screen
{
private:
public:
	MenuScreen(void);
	void setTextOriginAndPosition(sf::Text &text, int multiplier, sf::Vector2f screenDimensions);
	void centreSpriteOrigin(sf::Sprite & sprite);
	virtual int Run(sf::RenderWindow &window);
private:
	sf::Sprite playButton;
	sf::Sprite optionsButton;
	sf::Sprite quitButton;
	sf::Sprite blackGradient;
	sf::Sprite letterM;
	sf::Sprite letterA;
	sf::Sprite letterL1;
	sf::Sprite letterE1;
	sf::Sprite letterV;
	sf::Sprite letterO;
	sf::Sprite letterL2;
	sf::Sprite letterE2;
	sf::Sprite letterN;
	sf::Sprite letterC;
	sf::Sprite letterE3;
	sf::Sprite unfolded;

	sf::Vector2f m_origin, m_position, m_velocity, accel, m_natPosition;
	float m_dampingConst, m_springConst, m_mass;
};

#endif