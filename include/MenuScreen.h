#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>

class MenuScreen : public Screen
{
private:	
	float screenWidth;
	float screenHeight;
public:
	MenuScreen(void);
	void setTextOriginAndPosition(sf::Text &text, int multiplier, float screenWidth, float screenHeight);
	virtual int Run(sf::RenderWindow &window);
};

#endif