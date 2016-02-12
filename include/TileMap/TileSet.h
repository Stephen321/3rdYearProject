#ifndef TILE_SET_H
#define TILE_SET_H

#include "SFML\Graphics.hpp"

class TileSet {
public:
	typedef std::map<std::string, std::string> PropertyMap;
	typedef std::map<int, PropertyMap> TilePropertyMap;
	TileSet(int columns, int imageWidth, int imageHeight, int firstGid, int lastGid, std::string name, int tileWidth, int tileHeight, std::string path, TilePropertyMap tileProperties);
	sf::Texture * getTexture();
	int getFirstGid();
	int getLastGid();
	int getTileWidth();
	int getTileHeight();
	int getColumns();
	TilePropertyMap getTilePropertyMap();

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
	TilePropertyMap m_tileProperties;
};

#endif