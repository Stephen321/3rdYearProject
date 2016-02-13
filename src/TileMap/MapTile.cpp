#include "TileMap\MapTile.h"

MapTile::MapTile(sf::Sprite sprite, int gid, int width, int height, const PropertyMap* properties, int layer) :
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