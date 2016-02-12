#include "TileMap\MapLayer.h"

MapLayer::MapLayer(){
}

void MapLayer::setTiles(const vector<MapTile>& _tiles){
	tiles = _tiles;
}

void MapLayer::setType(MapLayerType type){
	m_type = type;
}

void MapLayer::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	if (m_type == MapLayerType::TileLayer){
		for (const MapTile & mapTile : tiles)
			target.draw(mapTile);
	}
	else if (m_type == MapLayerType::ObjectLayer){
		for (const MapObject & mapObject : objects)
			target.draw(mapObject);
	}
}

