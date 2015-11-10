#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>
#include "tmx\MapLoader.h"
#include "tmx2box2d.h"
#include "tmx\DebugShape.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Player.h"
#include "AI.h"
#include <memory>
#include "MyListener.h"
#include "Debug.h"

class GameScreen : public Screen
{
private:
public:
	GameScreen(void);
	virtual int Run(sf::RenderWindow &window);
};

#endif