#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "rapidjson\document.h"
#include <fstream>
#include "TileMap\TileSet.h"
#include "TileMap\MapLayer.h"

#include "Box2D\Box2D.h"
#include "CollisionFilters.h"
#include "Log.h"
#include <string>
#include <sstream>

using namespace tmx;
using namespace rapidjson;

class MapLoader {
public:
	MapLoader(const std::string& filePath = "");
	void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default, bool debug = false) const;
	void load(const std::string& mapName);
	sf::Vector2f isometricToOrthogonal(sf::Vector2f isoPos);
	sf::Vector2f orthogonalToIsometric(sf::Vector2f orthoPos);
	const vector<MapLayer>& getLayers();
	sf::Vector2f getPositionFromTileCoords(int x, int y);

private:
	std::string loadJSONDATA(std::string const & filename);
	int getGidTileSetIndex(int gid);
	void loadTileSets(const Document& document);
	PropertyMapMap loadTilePropertyMap(const Value & properties);
	void loadLayers(const Value & layers);
	void loadMapTiles(const Value & tiles, MapLayer& layer);
	MapTile createTile(int x, int y, int gid, int tileSetIndex);
	void loadMapObjects(const Value & objects, MapLayer& layer, sf::Color colour = sf::Color(128u, 128u, 128u));

	//tmx
	void createDebugGrid();
	sf::Color ColourFromHex(const char* hexStr) const;

	sf::Sprite m_sprite;
	std::string m_filePath;
	vector<MapLayer> m_layers;
	vector<TileSet> m_tileSets;
	int m_width;
	int m_height;
	int m_tileWidth;
	int m_tileHeight;
	bool m_visible;

	//tmx
	mutable sf::FloatRect m_bounds;
	mutable sf::Vector2f m_lastViewPos;
	sf::VertexArray m_gridVertices;
};

#endif