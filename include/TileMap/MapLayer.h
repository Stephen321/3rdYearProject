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


	typedef struct _TilePatch{
		_TilePatch() : visible(false), size(0){}
		bool visible;
		vector<MapTile> tiles;
		sf::FloatRect aabb;
		int size;
	}TilePatch;

	MapLayer(std::string name);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void setType(MapLayerType type);
	std::string getName() const;
	MapLayerType getType() const;
	vector<MapObject> objects;
	int getPatchSize() const;
	void setTilePatches(const vector<TilePatch> & tilePatches);
	void cull(sf::FloatRect bounds);

	//test
	vector<sf::RectangleShape> testShapes;

private:
	bool m_visible;
	int PATCH_SIZE;
	int m_tilePatchCount;
	vector<TilePatch> m_tilePatches;
	MapLayerType m_type;
	std::string m_name;
};

#endif