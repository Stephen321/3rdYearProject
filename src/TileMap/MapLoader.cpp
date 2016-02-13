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
	Document document;
	document.Parse<0>(jsonDATA.c_str());

	m_width = document["width"].GetInt();
	m_height = document["height"].GetInt();
	m_tileWidth = document["tilewidth"].GetInt();
	m_tileHeight = document["tileheight"].GetInt();

	loadTileSets(document);
	loadLayers(document["layers"]);
}

void MapLoader::loadTileSets(const Document& document){
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

		PropertyMapMap map;
		if (tileset->HasMember("tileproperties"))
			map = loadTilePropertyMap((*tileset)["tileproperties"]);
		m_tileSets.push_back(TileSet(columns, imageWidth, imageHeight, firstGid, lastGid, name, tileWidth, tileHeight, path, map));
	}
}

PropertyMapMap MapLoader::loadTilePropertyMap(const Value & properties){
	PropertyMapMap map;
	//loops through tile property objects
	for (Value::ConstMemberIterator propItr = properties.MemberBegin(); propItr != properties.MemberEnd(); ++propItr){
		int gid = std::stoi(propItr->name.GetString());
		//loops through individual tile properties
		for (Value::ConstMemberIterator tilePropItr = propItr->value.MemberBegin(); tilePropItr != propItr->value.MemberEnd(); ++tilePropItr){
			map[gid][tilePropItr->name.GetString()] = tilePropItr->value.GetString();
			std::cout << "setting Property for gid: " << gid << std::endl;
		}
	}
	return map;
}

void MapLoader::loadLayers(const Value & layers){
	for (Value::ConstValueIterator l = layers.Begin(); l != layers.End(); ++l){
		std::string name = (*l)["name"].GetString();
		MapLayer mapLayer(name);

		const std::string & type = (*l)["type"].GetString();
		if (type == "tilelayer"){
			mapLayer.setType(MapLayer::MapLayerType::TileLayer);

			///get tile gids
			vector<int> tileGids;
			const Value & tiles = (*l)["data"];
			for (Value::ConstValueIterator t = tiles.Begin(); t != tiles.End(); ++t){
				int gid = t->GetInt();
				tileGids.push_back(gid);
			}

			//set tiles
			mapLayer.tiles = loadMapTiles(tileGids);
		}
		else if (type == "objectgroup"){
			mapLayer.setType(MapLayer::MapLayerType::ObjectLayer);
			mapLayer.objects = loadMapObjects((*l)["objects"]);
		}

		m_layers.push_back(mapLayer);
	}
}

vector<MapTile> MapLoader::loadMapTiles(const vector<int>& tileGids){
	vector<MapTile> mapTiles;
	for (int y = 0; y < m_height; y++){
		for (int x = 0; x < m_width; x++){
			int gid = tileGids[x + (y*m_width)];
			if (gid > 0){
				int tileSetIndex = getGidTileSetIndex(gid);
				if (tileSetIndex != -1){
					mapTiles.push_back(createTile(x, y, gid, tileSetIndex));
				}
				else{
					std::cout << "Texture not found for gid: " << gid << std::endl;
				}
			}
		}
	}
	return mapTiles;
}

MapTile MapLoader::createTile(int x, int y, int gid, int tileSetIndex){
	const TileSet& currentTileSet = m_tileSets[tileSetIndex];
	int tileSetTileWidth = currentTileSet.getTileWidth();
	int tileSetTileHeight = currentTileSet.getTileHeight();
	int columns = currentTileSet.getColumns();
	int firstGid = currentTileSet.getFirstGid();

	const PropertyMap* propertyMap = currentTileSet.getPropertyMap(gid);
	sf::IntRect rect;
	rect.left = ((gid - firstGid) % columns) * tileSetTileWidth;
	rect.top = ((gid - firstGid) / columns) * tileSetTileHeight;
	rect.width = tileSetTileWidth;
	rect.height = tileSetTileHeight;

	sf::Sprite sprite(*currentTileSet.getTexture());
	sf::Vector2f position = getPositionFromTileCoords(x,y);
	sprite.setPosition(position);
	sprite.setTextureRect(rect);

	return MapTile(sprite, gid, tileSetTileWidth, tileSetTileHeight, propertyMap);//TODO: height
}

sf::Vector2f MapLoader::getPositionFromTileCoords(int x, int y){
	return sf::Vector2f((x - y) * m_tileWidth * 0.5f, (x + y) * m_tileHeight * 0.5f);
}

vector<MapObject> MapLoader::loadMapObjects(const Value & objects){
	vector<MapObject> mapObjects;
	for (Value::ConstValueIterator o = objects.Begin(); o != objects.End(); ++o){
		int width = (*o)["width"].GetDouble();
		int height = (*o)["height"].GetDouble();
		double x = (*o)["x"].GetDouble();
		double y = (*o)["y"].GetDouble();
		const Value & properties = (*o)["properties"];
		PropertyMap objectProperties;
		bool hasProperties = (properties.MemberEnd() - properties.MemberBegin() != 0);
		if (hasProperties){
			for (Value::ConstMemberIterator p = properties.MemberBegin(); p != properties.MemberEnd(); ++p)
				objectProperties[(*p).name.GetString()] = (*p).value.GetString();
		}

		std::string name = (*o)["name"].GetString();
		std::string type = (*o)["type"].GetString();
		sf::Vector2f position(x, y);
		sf::Vector2f size(width, height);

		MapObject mapObject(orthogonalToIsometric(position), size, name, type);
		if (hasProperties)
			mapObject.setObjectProperties(objectProperties);

		if ((*o).HasMember("polyline")){
			mapObject.setObjectType(MapObject::MapObjectType::Polyline);
			const Value & polylines = (*o)["polyline"];
			for (Value::ConstValueIterator polyline = polylines.Begin(); polyline != polylines.End(); ++polyline){
				double x = (*polyline)["x"].GetDouble();
				double y = (*polyline)["y"].GetDouble();
				sf::Vector2f point(x, y);
				mapObject.addPoint(orthogonalToIsometric(point));
			}
		}
		else{
			mapObject.setObjectType(MapObject::MapObjectType::Rectangle);
		}
		mapObjects.push_back(mapObject);
	}
	return mapObjects;
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