#ifndef MAP_TILE_H
#define MAP_TILE_H

#include "Properties.h"
#include "SFML\Graphics.hpp"

class MapTile : public sf::Drawable {
public:

	MapTile(sf::Sprite sprite, int gid, int width, int height, PropertyMap* properties = 0, int layer = 0);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void getPropertyString(const std::string& name, const std::string& value);
	std::string getPropertyString(const std::string& name);
	sf::Vector2f getPosition() const;
	sf::Vector2i getSize() const;

private:
	sf::Sprite m_sprite;
	PropertyMap* m_properties;
	sf::Vector2i m_size;
	int m_gid;
};

#endif