#include "TileMap\MapTile.h"

MapTile::MapTile(sf::Sprite sprite, int gid, int width, int height, PropertyMap* properties, int layer) :
m_sprite(sprite),
m_gid(gid),
m_width(width),
m_height(height),
m_properties(properties),
m_visible(true){

}

void MapTile::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(m_sprite);
}

void MapTile::getPropertyString(const std::string& name, const std::string& value){
	(*m_properties)[name] = value;
}

std::string MapTile::getPropertyString(const std::string& name){
	if (m_properties->find(name) != (*m_properties).end())
		return (*m_properties)[name];
	else
		return std::string();
}