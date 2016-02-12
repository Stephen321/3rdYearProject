#ifndef LAYER_H
#define LAYER_H

#include "MapTile.h"
#include "MapObject.h"
#include <vector>

using std::vector;

class MapLayer : public sf::Drawable {
public:
	enum class MapLayerType{
		TileLayer,
		ObjectLayer
	};

	MapLayer();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void setTiles(const vector<MapTile> & tiles);
	void setType(MapLayerType type);
	vector<MapTile> tiles;
	vector<MapObject> objects;

private:
	bool m_visible;
	const sf::Texture * m_tileSet;
	MapLayerType m_type;
};

#endif