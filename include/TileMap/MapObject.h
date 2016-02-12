#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "SFML\Graphics.hpp"

class MapObject : public sf::Drawable {
public:
	MapObject();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

private:
	sf::Vector2f m_position;
	bool m_visible;
};

#endif