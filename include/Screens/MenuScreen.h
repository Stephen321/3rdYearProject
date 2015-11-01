#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>

class MenuScreen : public Screen
{
private:
public:
	MenuScreen(void);
	void setTextOriginAndPosition(sf::Text &text, int multiplier, sf::Vector2f screenDimensions);
	virtual int Run(sf::RenderWindow &window);
};

#endif