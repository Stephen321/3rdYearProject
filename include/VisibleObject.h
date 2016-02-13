#ifndef VISIBLEOBJECT_H
#define VISIBLEOBJECT_H
#include "SFML\Graphics.hpp"
#include "TileMap\Helpers.h"
using namespace Helpers::Tiled;

class VisibleObject : public sf::Drawable{
public:
	virtual sf::Vector2f getPosition() const;
protected:
	sf::Vector2f m_position;
};

#endif