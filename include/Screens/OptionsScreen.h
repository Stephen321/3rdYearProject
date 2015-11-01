#ifndef OPTIONSSCREEN_H
#define OPTIONSSCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>

class OptionsScreen : public Screen
{
private:
public:
	OptionsScreen(void);
	void setTextOriginAndPosition(sf::Text &text, int multiplier, sf::Vector2f screenDimensions);
	virtual int Run(sf::RenderWindow &window);
};

#endif