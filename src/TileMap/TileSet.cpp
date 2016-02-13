#include "TileMap\TileSet.h"

TileSet::TileSet(int columns, int imageWidth, int imageHeight, int firstGid, int lastGid, std::string name, int tileWidth, int tileHeight, std::string path, PropertyMapMap tileProperties) :
m_columns(columns),
m_imageWidth(imageWidth),
m_imageHeight(imageHeight),
m_firstGid(firstGid),
m_lastGid(lastGid),
m_name(name),
m_tileWidth(tileWidth),
m_tileHeight(tileHeight),
m_imagePath(path),
m_tileProperties(tileProperties){
	m_texture.loadFromFile(m_imagePath);
}

const sf::Texture * TileSet::getTexture() const{
	return &m_texture;
}

int TileSet::getFirstGid() const{
	return m_firstGid;
}

int TileSet::getLastGid() const{
	return m_lastGid;
}

int TileSet::getTileWidth() const{
	return m_tileWidth;
}

int TileSet::getTileHeight() const{
	return m_tileHeight;
}

int TileSet::getColumns() const{
	return m_columns;
}

const PropertyMap* TileSet::getPropertyMap(int gid) const{
	if (m_tileProperties.find(gid) != m_tileProperties.end())
		return &m_tileProperties.at(gid);
	return 0;
}