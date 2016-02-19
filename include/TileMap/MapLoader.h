#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "rapidjson\document.h"
#include <fstream>
#include "TileMap\TileSet.h"
#include "TileMap\MapLayer.h"

#include "Box2D\Box2D.h"
#include "CollisionFilters.h"
#include "Pathfinding\Pathfinder.h"
#include "Debug.h"
#include "Log.h"
#include <string>
#include <sstream>

using namespace tmx;
using namespace rapidjson;

class MapLoader : sf::Drawable{
public:
	MapLoader();
	void init(const std::string& filePath = "", Pathfinder * _pathFinder = 0);
	void load(const std::string& mapName);
	const vector<MapLayer>& getLayers();
	void Draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default);

	//helpers
	static sf::Vector2f getPositionFromTileCoords(int x, int y);
	static sf::Vector2i getTileCoordsFromPos(sf::Vector2f screenPos);
	static sf::Vector2f isometricToOrthogonal(sf::Vector2f isoPos);
	static sf::Vector2f orthogonalToIsometric(sf::Vector2f orthoPos);

private:
	void cull(sf::RenderTarget& target);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const;
	std::string loadJSONDATA(std::string const & filename);
	int getGidTileSetIndex(int gid);
	void loadTileSets(const Document& document);
	PropertyMapMap loadTilePropertyMap(const Value & properties, int firstGid);
	void loadLayers(const Value & layers);
	void loadMapTiles(const Value & tiles, MapLayer& layer);
	MapTile createTile(int x, int y, int gid, int tileSetIndex);
	void loadPathfindingNodes(const Value & tiles, MapLayer& layer);
	void loadMapObjects(const Value & objects, MapLayer& layer, sf::Color colour = sf::Color(128u, 128u, 128u));

	Pathfinder * pathFinder;
	//tmx
	void createDebugGrid();
	sf::Color ColourFromHex(const char* hexStr) const;

	sf::Sprite m_sprite;
	std::string m_filePath;
	vector<MapLayer> m_layers;
	vector<TileSet> m_tileSets;
	int m_width;
	int m_height;
	static int m_tileWidth;
	static int m_tileHeight;
	bool m_visible;

	//tmx
	mutable sf::FloatRect m_bounds;
	mutable sf::Vector2f m_lastViewPos;
	sf::VertexArray m_gridVertices;
};

#endif