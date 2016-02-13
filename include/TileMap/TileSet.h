#ifndef TILE_SET_H
#define TILE_SET_H

#include "Properties.h"
#include "SFML\Graphics.hpp"

class TileSet {
public:
	TileSet(int columns, int imageWidth, int imageHeight, int firstGid, int lastGid, std::string name, int tileWidth, int tileHeight, std::string path, PropertyMapMap tileProperties);
	const sf::Texture* getTexture() const;
	int getFirstGid() const;
	int getLastGid() const;
	int getTileWidth() const;
	int getTileHeight() const;
	int getColumns() const;
	const PropertyMap* getPropertyMap(int gid) const;

private:
	int m_columns;
	int m_imageWidth;
	int m_imageHeight;
	int m_firstGid;
	int m_lastGid;
	std::string m_name;
	int m_tileWidth;
	int m_tileHeight;
	std::string m_imagePath;
	sf::Texture m_texture;
	PropertyMapMap m_tileProperties;
};

#endif