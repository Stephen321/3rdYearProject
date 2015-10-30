#ifndef CREDITSSCREEN_H
#define CREDITSSCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>

class CreditsScreen : public Screen
{
private:	
public:
	CreditsScreen(void);
	virtual int Run(sf::RenderWindow &window);
};

#endif