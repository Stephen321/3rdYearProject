#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <SFML\Graphics.hpp>

class HelperFunctions{
public:
	static b2Vec2 SfToBoxVec(const sf::Vector2f& vec);
	static sf::Vector2f BoxToSfVec(const b2Vec2& vec);
	static float SfToBoxFloat(float val);
	static float BoxToSfFloat(float val);
	static float SfToBoxAngle(float degrees);
	static float BoxToSfAngle(float rads);
};

#endif