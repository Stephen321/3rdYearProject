#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D\Collision\Shapes\b2PolygonShape.h>

#include "Screen.hpp"
#include "Player.h"
#include "AI.h"
#include "MyListener.h"
#include "Debug.h"
#include "Rock.h"
#include "Bush.h"
#include "Tree.h"
#include "TallGrass.h"
#include "SoundManager.h"
#include "TileMap\BodyCreator.h"
#include "Pathfinding\Pathfinder.h"

class GameScreen : public Screen
{
public:
	GameScreen(MapLoader * ml, Pathfinder * pf);
	virtual int Run(sf::RenderWindow &window);
private:
	Pathfinder * m_pathFinder;
	MapLoader * m_mapLoader;
};

#endif