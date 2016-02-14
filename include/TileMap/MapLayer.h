#ifndef LAYER_H
#define LAYER_H

#include "MapTile.h"
#include "MapObject.h"
#include <vector>

using std::vector;
using namespace tmx;

class MapLayer : public sf::Drawable {
public:
	enum class MapLayerType{
		TileLayer,
		ObjectGroup
	};

	MapLayer(std::string name);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void setType(MapLayerType type);
	std::string getName() const;
	MapLayerType getType() const;
	vector<MapTile> tiles;
	vector<MapObject> objects;

private:
	bool m_visible;
	const sf::Texture * m_tileSet;
	MapLayerType m_type;
	std::string m_name;
};

#endif