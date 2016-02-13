#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "rapidjson\document.h"
#include <fstream>
#include "TileMap\TileSet.h"
#include "TileMap\MapLayer.h"

#include "Box2D\Box2D.h"
#include "CollisionFilters.h"

using namespace rapidjson;

class MapLoader : public sf::Drawable {
public:
	MapLoader(const std::string& filePath = "");
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
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
	vector<MapTile> loadMapTiles(const vector<int>& tileGids);
	MapTile createTile(int x, int y, int gid, int tileSetIndex);
	vector<MapObject> loadMapObjects(const Value & objects);

	sf::Sprite m_sprite;
	std::string m_filePath;
	vector<MapLayer> m_layers;
	vector<TileSet> m_tileSets;
	int m_width;
	int m_height;
	int m_tileWidth;
	int m_tileHeight;
	bool m_visible;
};

#endif