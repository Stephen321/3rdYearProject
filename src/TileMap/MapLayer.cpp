#include "TileMap\MapLayer.h"
#include "Debug.h" //test

MapLayer::MapLayer(std::string name):
m_name(name),
m_visible(true),
PATCH_SIZE(10),
m_tilePatches(0){
}

void MapLayer::setType(MapLayerType type){
	m_type = type;
}

std::string MapLayer::getName() const{
	return m_name;
}

void MapLayer::cull(sf::FloatRect bounds){
	if (m_type == MapLayerType::TileLayer){
		int tilePatchCount = m_tilePatches.size();
		for (int i = 0; i < tilePatchCount; i++){
			if (bounds.intersects(m_tilePatches[i].aabb))
				m_tilePatches[i].visible = true;
			else
				m_tilePatches[i].visible = false;
		}
		sf::RectangleShape test;
		test.setSize(sf::Vector2f(bounds.width, bounds.height));
		test.setFillColor(sf::Color(255, 0,0, 25));
		test.setOutlineColor(sf::Color::Magenta);
		test.setOutlineThickness(8.f);
		test.setPosition(bounds.left, bounds.top);
		testShapes.back() = test;
	}
}

void MapLayer::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	if (m_type == MapLayerType::TileLayer){
		int tilePatchCount = m_tilePatches.size();
		int testDrawCalls = 0;
		for (int i = 0; i < tilePatchCount; i++){
			if (m_tilePatches[i].visible){
				testDrawCalls++;
				const vector<MapTile> & tiles =	m_tilePatches[i].tiles;
				for (int j = 0; j < m_tilePatches[i].size; j++){
					target.draw(tiles[j]);
				}
			}
		}
		if (Debug::displayInfo)
			std::cout << "Patches drawing: " << testDrawCalls << std::endl;
	}

	//test 
	if (Debug::displayInfo){
		for (auto s : testShapes)
			target.draw(s);
	}
}

MapLayer::MapLayerType MapLayer::getType () const{
	return m_type;
}


void MapLayer::setTilePatches(const vector<TilePatch> & tilePatches){
	m_tilePatches = tilePatches;
	int tilePatchCount = m_tilePatches.size();
	for (int i = 0; i < tilePatchCount; i++){ //loop through every patch
		const vector<MapTile> & tiles = m_tilePatches[i].tiles; //get tiles in patch
		int tileCount = tiles.size();
		if (tileCount != 0) { //if its not empty
			//set up 
			sf::Vector2f position = tiles[0].getPosition();
			sf::Vector2i size = tiles[0].getSize();
			float tileRatio = size.x / size.y;
			sf::FloatRect bounds;
			int minX = std::numeric_limits<int>::max(), maxX = std::numeric_limits<int>::min(),
				minY = std::numeric_limits<int>::max(), maxY = std::numeric_limits<int>::min();

			for (int j = 0; j < tileCount; j++){ //loop through every tile
				position = tiles[j].getPosition();
				size = sf::Vector2i(tiles[j].getSize().x / tileRatio, tiles[j].getSize().y / tileRatio);
				if (position.x < minX){
					minX = position.x;
				}
				if (position.y < minY){
					minY = position.y;
				}
				if (position.x + size.x > maxX){
					maxX = position.x + size.x;
				}
				if (position.y + size.y > maxY){
					maxY = position.y + size.y;
				}
			}
			bounds.left = minX;
			bounds.top = minY;
			bounds.width = maxX - minX;
			bounds.height = maxY - minY;

			m_tilePatches[i].aabb = bounds;
			sf::RectangleShape test;
			test.setSize(sf::Vector2f(bounds.width, bounds.height));
			test.setFillColor(sf::Color::Transparent);
			test.setOutlineColor(sf::Color::Blue);
			test.setOutlineThickness(4.f);
			test.setPosition(bounds.left, bounds.top);
			testShapes.push_back(test);
		}
	}
}

int MapLayer::getPatchSize() const{
	return PATCH_SIZE;
}