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
		//times 3 so that doesnt cull tiles that are shown
		bounds.left -= bounds.width / 2.f;
		bounds.top -= bounds.height / 2.f;
		bounds.width *= 2.f;
		bounds.height *= 2.5f;
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
	for (int i = 0; i < tilePatchCount; i++){
		const vector<MapTile> & tiles = m_tilePatches[i].tiles;
		sf::FloatRect bounds;
		if (!tiles.empty() && bounds.width == 0.f){
			bounds.left = tiles[0].getPosition().x;
			bounds.top = tiles[0].getPosition().y;
			bounds.width = tiles[0].getSize().x;
			bounds.height = tiles[0].getSize().y;
		}
		for (int j = 0; j < m_tilePatches[i].size; j++){
			if (tiles[j].getPosition().x < bounds.left){
				bounds.left = tiles[j].getPosition().x;
			}
			if (tiles[j].getPosition().y < bounds.top){
				bounds.top = tiles[j].getPosition().y;
			} //tiles[j].getPosition().y
			if (fabs(bounds.left -(tiles[j].getPosition().x + tiles[j].getSize().x)) > bounds.width){
				bounds.width = fabs(bounds.left - (tiles[j].getPosition().x + tiles[j].getSize().x));
			}
			if (fabs(bounds.top - (tiles[j].getPosition().y + tiles[j].getSize().y)) > bounds.height){
				bounds.height = fabs(bounds.top - (tiles[j].getPosition().y + tiles[j].getSize().y));
			}
		}
		m_tilePatches[i].aabb = bounds;
		sf::RectangleShape test;
		test.setSize(sf::Vector2f(bounds.width, bounds.height));
		test.setFillColor(sf::Color::Transparent);
		test.setOutlineColor(sf::Color::Blue);
		test.setOutlineThickness(4.f);
		test.setPosition(bounds.left, bounds.top);
		if (bounds.width != 0.f)
 			testShapes.push_back(test);
	}
	testShapes.push_back(sf::RectangleShape());
}

int MapLayer::getPatchSize() const{
	return PATCH_SIZE;
}