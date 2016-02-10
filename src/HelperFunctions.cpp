#include "HelperFunctions.h"

namespace
{
	//this scales sf units into box2D metres
	//value is units per metre.
	const float worldScale = 100.f;
	//used in point calcs. Don't change this.
	const float pointTolerance = 0.1f;
}

b2Vec2 HelperFunctions::SfToBoxVec(const sf::Vector2f& vec)
{
	return b2Vec2(vec.x / worldScale, -vec.y / worldScale);
}

sf::Vector2f HelperFunctions::BoxToSfVec(const b2Vec2& vec)
{
	return sf::Vector2f(vec.x, -vec.y) * worldScale;
}

float HelperFunctions::SfToBoxFloat(float val)
{
	return val / worldScale;
}

float HelperFunctions::BoxToSfFloat(float val)
{
	return val * worldScale;
}

float HelperFunctions::SfToBoxAngle(float degrees)
{
	return -degrees * 0.0174533f;
}

float HelperFunctions::BoxToSfAngle(float rads)
{
	return -rads * 57.29578f;
}