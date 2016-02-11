#ifndef TILE_H
#define TILE_H

#include "SFML\Graphics.hpp"

class Tile : public sf::Drawable {
public:
	Tile(sf::Texture texture, sf::Vector2f position);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

private:
	sf::Sprite m_sprite;
	int m_height;
	int m_weight;
	int m_id;
	bool m_walkable;
};

#endif