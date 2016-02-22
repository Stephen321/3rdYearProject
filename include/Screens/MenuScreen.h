#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <iostream>
#include <memory>
#include "Screen.hpp"
#include "GameData.h"
#include "SpringObject.h"

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
	sf::Sprite unfolded;
	sf::Sprite titleLetters;

	std::vector<SpringObject> springs;
};

#endif