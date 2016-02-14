#include "TileMap\MapLayer.h"

MapLayer::MapLayer(std::string name):
m_name(name),
m_visible(true){
}

void MapLayer::setType(MapLayerType type){
	m_type = type;
}

std::string MapLayer::getName() const{
	return m_name;
}

void MapLayer::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	if (m_type == MapLayerType::TileLayer){
		for (const MapTile & mapTile : tiles)
			target.draw(mapTile);
	}
}

MapLayer::MapLayerType MapLayer::getType() const{
	return m_type;
}