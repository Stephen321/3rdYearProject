#include "TileMap\MapLoader.h"

#include <iostream> //test

MapLoader::MapLoader(const std::string& filePath) :
m_filePath(filePath){

}

std::string MapLoader::loadJSONDATA(std::string const & filename){
	std::string data = "";
	std::ifstream file(filename);
	std::string line;
	if (file.is_open()){
		while (std::getline(file, line)){
			data.append(line);
		}
	}
	return data;
}

void MapLoader::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	for (MapLayer l : m_layers)
		target.draw(l);
}

void MapLoader::load(const std::string& mapName){
	const std::string & jsonDATA = loadJSONDATA(m_filePath + mapName);
	rapidjson::Document document;
	document.Parse<0>(jsonDATA.c_str());

	m_width = document["width"].GetInt();
	m_height = document["height"].GetInt();
	m_tileWidth = document["tilewidth"].GetInt();
	m_tileHeight = document["tileheight"].GetInt();

	//load tilesets
	const Value & tilesets = document["tilesets"];
	for (Value::ConstValueIterator tileset = tilesets.Begin(); tileset != tilesets.End(); ++tileset){
		int columns = (*tileset)["columns"].GetInt();
		int imageWidth = (*tileset)["imagewidth"].GetInt();
		int imageHeight = (*tileset)["imageheight"].GetInt();
		int firstGid = (*tileset)["firstgid"].GetInt();
		int lastGid = firstGid + (*tileset)["tilecount"].GetInt() - 1;
		std::string name = (*tileset)["name"].GetString();
		int tileWidth = (*tileset)["tilewidth"].GetInt();
		int tileHeight = (*tileset)["tileheight"].GetInt();
		std::string path = m_filePath + (*tileset)["image"].GetString();

		TileSet::TilePropertyMap map;
		if (tileset->HasMember("tileproperties")){
			const Value & properties = (*tileset)["tileproperties"];
			//loops through tile property objects
			for (Value::ConstMemberIterator propItr = properties.MemberBegin(); propItr != properties.MemberEnd(); ++propItr){
				int gid = std::stoi(propItr->name.GetString());
				//loops through individual tile properties
				for (Value::ConstMemberIterator tilePropItr = propItr->value.MemberBegin(); tilePropItr != propItr->value.MemberEnd(); ++tilePropItr){
					map[gid][tilePropItr->name.GetString()] = tilePropItr->value.GetString();
				}
			}
		}
		m_tileSets.push_back(TileSet(columns, imageWidth, imageHeight, firstGid, lastGid, name, tileWidth, tileHeight, path, map));
	}

	const Value & layers = document["layers"];
	for (Value::ConstValueIterator l = layers.Begin(); l != layers.End(); ++l){
		MapLayer mapLayer;

		const std::string & type = (*l)["type"].GetString();
		if (type == "tilelayer"){
			mapLayer.setType(MapLayer::MapLayerType::TileLayer);

			///get tile gids
			vector<int> tileGids;
			const Value & tiles = (*l)["data"];
			for (Value::ConstValueIterator tile = tiles.Begin(); tile != tiles.End(); ++tile){
				int gid = tile->GetInt();
				tileGids.push_back(gid);					
			}

			//create tiles
			vector<MapTile> mapTiles;
			for (int y = 0; y < m_height; y++){
				for (int x = 0; x < m_width; x++){
					int gid = tileGids[x + (y*m_width)];
					if (gid > 0){
						sf::Sprite sprite;
						int tileSetIndex = getGidTileSetIndex(gid);
						if (tileSetIndex != -1){
							int tileSetTileWidth = m_tileSets[tileSetIndex].getTileWidth();
							int tileSetTileHeight = m_tileSets[tileSetIndex].getTileHeight();
							int columns = m_tileSets[tileSetIndex].getColumns();
							int firstGid = m_tileSets[tileSetIndex].getFirstGid();
							TileSet::TilePropertyMap tilePropertyMap = m_tileSets[tileSetIndex].getTilePropertyMap();
							TileSet::PropertyMap propertyMap;
							if (tilePropertyMap.find(gid) != tilePropertyMap.end()){
								propertyMap = tilePropertyMap.at(gid);
							}
							sf::IntRect rect;
							rect.left = ((gid - firstGid) % columns) * tileSetTileWidth;
							rect.top = ((gid - firstGid) / columns) * tileSetTileHeight;
							rect.width = tileSetTileWidth;
							rect.height = tileSetTileHeight;

							sf::Vector2f position = sf::Vector2f((x - y) * m_tileWidth * 0.5f , (x + y) * m_tileHeight * 0.5f );
							sprite.setPosition(position);
							sprite.setTexture(*m_tileSets[tileSetIndex].getTexture());
							sprite.setTextureRect(rect);
							mapTiles.push_back(MapTile(sprite, gid, tileSetTileWidth, tileSetTileHeight, propertyMap));//TODO: height
						}
						else{
							std::cout << "Texture not found for gid: " << gid << std::endl;
						}
					}
				}
			}
			mapLayer.setTiles(mapTiles);
		}
		else if (type == "objectgroup")
			mapLayer.setType(MapLayer::MapLayerType::ObjectLayer);

		m_layers.push_back(mapLayer);
	}
}

int MapLoader::getGidTileSetIndex(int gid){
	const int SIZE = m_tileSets.size();
	for (int i = 0; i < SIZE; i++){
		if (gid >= m_tileSets[i].getFirstGid() && gid <= m_tileSets[i].getLastGid())
			return i;
	}
	return -1;
}

sf::Vector2f MapLoader::isometricToOrthogonal(sf::Vector2f isoPos){
	sf::Vector2f orthoPos;
	orthoPos.x = (2 * isoPos.y + isoPos.x) * 0.5f;
	orthoPos.y = (2 * isoPos.y - isoPos.x) * 0.5f;
	return orthoPos;
}

sf::Vector2f MapLoader::orthogonalToIsometric(sf::Vector2f orthoPos){
	sf::Vector2f isoPos;
	isoPos.x = (orthoPos.x - orthoPos.y);
	isoPos.y = (orthoPos.x + orthoPos.y) * 0.5f;
	return isoPos;
}

const vector<MapLayer>& MapLoader::getLayers(){
	return m_layers;
}