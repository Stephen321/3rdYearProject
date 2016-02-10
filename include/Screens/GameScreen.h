#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <iostream>
#include "Screen.hpp"

#include <SFML/Graphics.hpp>
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Player.h"
#include "AI.h"
#include <memory>
#include "MyListener.h"
#include "Debug.h"
#include "Rock.h"
#include "SoundManager.h"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Fixture.h>

class GameScreen : public Screen
{
public:
	GameScreen();
	virtual int Run(sf::RenderWindow &window);
};

#endif