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

void MapLoader::draw(sf::RenderTarget& target, sf::RenderStates state, bool debug) const{
	sf::View view = target.getView();
	if (view.getCenter() != m_lastViewPos)
	{
		sf::FloatRect bounds;
		bounds.left = view.getCenter().x - (view.getSize().x / 2.f);
		bounds.top = view.getCenter().y - (view.getSize().y / 2.f);
		bounds.width = view.getSize().x;
		bounds.height = view.getSize().y;

		//add a tile border to prevent gaps appearing
		bounds.left -= static_cast<float>(m_tileWidth);
		bounds.top -= static_cast<float>(m_tileHeight);
		bounds.width += static_cast<float>(m_tileWidth * 2);
		bounds.height += static_cast<float>(m_tileHeight * 2);
		m_bounds = bounds;
		//for (auto& layer : m_layers)
			//layer.Cull(m_bounds);
	}
	m_lastViewPos = view.getCenter();

	for (const MapLayer& l : m_layers)
		target.draw(l);

	if (debug){
		for (auto layer : m_layers)
		{
			if (layer.getType() == MapLayer::MapLayerType::ObjectGroup)
			{
				for (const MapObject& object : layer.objects)
				if (m_bounds.intersects(object.GetAABB()))
					object.DrawDebugShape(target);
			}
		}
		target.draw(m_gridVertices);
	}
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
	createDebugGrid();
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
			loadMapTiles((*l)["data"], mapLayer);
		}
		else if (type == "objectgroup"){
			mapLayer.setType(MapLayer::MapLayerType::ObjectGroup);
			sf::Color colour = sf::Color(128u,128u,128u);
			if ((*l).HasMember("color")){
				std::string hex = (*l)["color"].GetString();
				std::remove(hex.begin(), hex.end(), '#');
				hex.pop_back();
				colour = ColourFromHex(hex.c_str());
			}
			loadMapObjects((*l)["objects"], mapLayer, colour);
		}

		m_layers.push_back(mapLayer);
	}
}

void MapLoader::loadMapTiles(const Value & tiles, MapLayer& layer){
	///get tile gids
	vector<int> tileGids;
	for (Value::ConstValueIterator t = tiles.Begin(); t != tiles.End(); ++t){
		int gid = t->GetInt();
		tileGids.push_back(gid);
	}

	for (int y = 0; y < m_height; y++){
		for (int x = 0; x < m_width; x++){
			int gid = tileGids[x + (y*m_width)];
			if (gid > 0){
				int tileSetIndex = getGidTileSetIndex(gid);
				if (tileSetIndex != -1){
					layer.tiles.push_back(createTile(x, y, gid, tileSetIndex));
				}
				else{
					std::cout << "Texture not found for gid: " << gid << std::endl;
				}
			}
		}
	}
}

MapTile MapLoader::createTile(int x, int y, int gid, int tileSetIndex){
	const TileSet& currentTileSet = m_tileSets[tileSetIndex];
	int tileSetTileWidth = currentTileSet.getTileWidth();
	int tileSetTileHeight = currentTileSet.getTileHeight();
	int columns = currentTileSet.getColumns();
	int firstGid = currentTileSet.getFirstGid();

	PropertyMap* propertyMap = const_cast<PropertyMap*>(currentTileSet.getPropertyMap(gid));
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


//tmx loader xml converted to json
void MapLoader::loadMapObjects(const Value & objects, MapLayer& layer, sf::Color colour){
	for (Value::ConstValueIterator o = objects.Begin(); o != objects.End(); ++o){
		MapObject object;
		//set position
		sf::Vector2f position((*o)["x"].GetDouble(),(*o)["y"].GetDouble());
		object.SetPosition(isometricToOrthogonal(position));

		//set size if specified
		if ((*o).HasMember("polygon") || (*o).HasMember("polyline"))
		{
			if ((*o).HasMember("polygon"))
			{
				const Value & child = (*o)["polygon"];
				for (Value::ConstValueIterator p = child.Begin(); p != child.End(); ++p){
					double x = (*p)["x"].GetDouble();
					double y = (*p)["y"].GetDouble();
					sf::Vector2f point(x, y);
					object.AddPoint(isometricToOrthogonal(point));
				}
				object.SetShapeType(MapObjectShape::Polygon);
			}
			else
			{
				const Value & child = (*o)["polyline"];
				for (Value::ConstValueIterator p = child.Begin(); p != child.End(); ++p){
					double x = (*p)["x"].GetDouble();
					double y = (*p)["y"].GetDouble();
					sf::Vector2f point(x, y);
					object.AddPoint(isometricToOrthogonal(point));
				}
				object.SetShapeType(MapObjectShape::Polyline);
			}
		}
		//else parse poly points
		else if ((*o).HasMember("width") && (*o).HasMember("height"))
		{
			sf::Vector2f size((*o)["width"].GetDouble(),
				(*o)["height"].GetDouble());
			if ((*o).HasMember("ellipse"))
			{
				//add points to make ellipse
				const float x = size.x / 2.f;
				const float y = size.y / 2.f;
				const float tau = 6.283185f;
				const float step = tau / 16.f; //number of points to make up ellipse
				for (float angle = 0.f; angle < tau; angle += step)
				{
					sf::Vector2f point(x + x * cos(angle), y + y * sin(angle));
					object.AddPoint(isometricToOrthogonal(point));
				}

				if (size.x == size.y) object.SetShapeType(Circle);
				else object.SetShapeType(MapObjectShape::Ellipse);
			}
			else //add points for rectangle to use in intersection testing
			{
				object.AddPoint(isometricToOrthogonal(sf::Vector2f()));
				object.AddPoint(isometricToOrthogonal(sf::Vector2f(size.x, 0.f)));
				object.AddPoint(isometricToOrthogonal(sf::Vector2f(size.x, size.y)));
				object.AddPoint(isometricToOrthogonal(sf::Vector2f(0.f, size.y)));
			}
			object.SetSize(size);
		}

		//parse object node property values
		if ((*o).HasMember("properties"))
		{
			const Value & properties = (*o)["properties"];
			for (Value::ConstMemberIterator p = properties.MemberBegin(); p != properties.MemberEnd(); ++p){
				object.SetProperty((*p).name.GetString(), (*p).value.GetString());
				LOG("Set object property " + name + " with value " + value, Logger::Type::Info);
			}
		}

		//set object properties
		if ((*o).HasMember("name")) object.SetName((*o)["name"].GetString());
		if ((*o).HasMember("type")) object.SetType((*o)["type"].GetString());
		if ((*o).HasMember("visible")) object.SetVisible((*o)["visible"].GetBool());

		object.SetParent(layer.getName());

		object.CreateDebugShape(colour);

		//creates line segments from any available points
		object.CreateSegments();

		//add objects to vector
		layer.objects.push_back(object);
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


//tmx 
void MapLoader::createDebugGrid()
{
	float tileRatio = static_cast<float>(m_tileWidth) / static_cast<float>(m_tileHeight);
	sf::Color debugColour(0u, 0u, 0u, 120u);
	float mapHeight = static_cast<float>(m_tileHeight * m_height);
	for (int x = 0; x <= m_width; x += 2)
	{
		float posX = static_cast<float>(x * (m_tileWidth / tileRatio));
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(posX, 0.f)), debugColour));
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(posX, mapHeight)), debugColour));
		posX += static_cast<float>(m_tileWidth) / tileRatio;
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(posX, mapHeight)), debugColour));
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(posX, 0.f)), debugColour));
		posX += static_cast<float>(m_tileWidth) / tileRatio;
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(posX, 0.f)), debugColour));
	}
	float mapWidth = static_cast<float>(m_tileWidth * (m_width / tileRatio));
	for (int y = 0; y <= m_height; y += 2)
	{
		float posY = static_cast<float>(y *m_tileHeight);
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(0.f, posY)), debugColour));
		posY += static_cast<float>(m_tileHeight);
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(0.f, posY)), debugColour));
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(mapWidth, posY)), debugColour));
		posY += static_cast<float>(m_tileHeight);
		m_gridVertices.append(sf::Vertex(isometricToOrthogonal(sf::Vector2f(mapWidth, posY)), debugColour));
	}

	m_gridVertices.setPrimitiveType(sf::LinesStrip);
}

sf::Color MapLoader::ColourFromHex(const char* hexStr) const
{
	//TODO proper checking valid string length
	unsigned int value, r, g, b;
	std::stringstream input(hexStr);
	input >> std::hex >> value;

	r = (value >> 16) & 0xff;
	g = (value >> 8) & 0xff;
	b = value & 0xff;

	return sf::Color(r, g, b);
}

sf::Vector2f MapLoader::isometricToOrthogonal(sf::Vector2f isoPos){
	sf::Vector2f orthoPos;
	orthoPos.x = (isoPos.x - isoPos.y);
	orthoPos.y = (isoPos.x + isoPos.y) * 0.5f;
	return orthoPos;
}

sf::Vector2f MapLoader::orthogonalToIsometric(sf::Vector2f orthoPos){
	sf::Vector2f isoPos;
	isoPos.x = (2 * orthoPos.y + orthoPos.x) * 0.5f;
	isoPos.y = (2 * orthoPos.y - orthoPos.x) * 0.5f;
	return isoPos;
}

const vector<MapLayer>& MapLoader::getLayers(){
	return m_layers;
}