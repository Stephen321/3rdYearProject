#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>
#include "tmx\MapLoader.h"
#include "Player.h"

class GameScreen : public Screen
{
private:
	float movement_step;
	float posx;
	float posy;
	sf::RectangleShape Rectangle;
public:
	GameScreen(void);
	virtual int Run(sf::RenderWindow &window);
};

#endif