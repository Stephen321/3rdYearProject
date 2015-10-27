#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>

class MenuScreen : public Screen
{
private:
	int alpha_max;
	int alpha_div;
	bool playing;
public:
	MenuScreen(void);
	virtual int Run(sf::RenderWindow &window);
};

#endif