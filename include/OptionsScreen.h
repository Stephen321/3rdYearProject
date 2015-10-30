#ifndef OPTIONSSCREEN_H
#define OPTIONSSCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>

class OptionsScreen : public Screen
{
private:
	int alpha_max;
	int alpha_div;
	bool playing;
public:
	OptionsScreen(void);
	virtual int Run(sf::RenderWindow &window);
};

#endif