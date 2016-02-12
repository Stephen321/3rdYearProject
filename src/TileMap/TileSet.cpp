#include "TileMap\TileSet.h"

TileSet::TileSet(int columns, int imageWidth, int imageHeight, int firstGid, int lastGid, std::string name, int tileWidth, int tileHeight, std::string path, TileSet::TilePropertyMap tileProperties) :
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

sf::Texture * TileSet::getTexture(){
	return &m_texture;
}

int TileSet::getFirstGid(){
	return m_firstGid;
}

int TileSet::getLastGid(){
	return m_lastGid;
}

int TileSet::getTileWidth(){
	return m_tileWidth;
}

int TileSet::getTileHeight(){
	return m_tileHeight;
}

int TileSet::getColumns(){
	return m_columns;
}

TileSet::TilePropertyMap TileSet::getTilePropertyMap(){
	return m_tileProperties;
}