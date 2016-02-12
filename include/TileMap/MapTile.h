#ifndef MAP_TILE_H
#define MAP_TILE_H

#include "SFML\Graphics.hpp"

class MapTile : public sf::Drawable {
public:
	typedef std::map<std::string, std::string> PropertyMap;

	MapTile(sf::Sprite sprite, int gid, int width, int height, PropertyMap properties, int layer = 0);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

private:
	sf::Sprite m_sprite;
	PropertyMap m_properties;
	int m_height;
	int m_width;
	int m_gid;
	bool m_walkable;
	bool m_visible;
};

#endif