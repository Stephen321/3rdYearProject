#include "TileMap\MapTile.h"

MapTile::MapTile(sf::Sprite sprite, int gid, int width, int height, PropertyMap* properties, int layer) :
m_sprite(sprite),
m_gid(gid),
m_size(sf::Vector2i(width, height)),
m_properties(properties){

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

sf::Vector2f MapTile::getPosition() const{
	return m_sprite.getPosition();
}

sf::Vector2i MapTile::getSize() const{
	return m_size;
}