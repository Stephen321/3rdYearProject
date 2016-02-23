#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <iostream>
#include <queue>
#include "Node.h"

using namespace std;

class Pathfinder {
public:           
	Pathfinder();
	~Pathfinder();
	void setSize(int width, int height);
	void addNode(int x, int y, bool walkable, const std::string& area, const sf::Vector2f& position);
	std::vector<sf::Vector2f> findPath(const sf::Vector2i& startPos, const sf::Vector2i& goalPos, bool ignoreWalkable = false);
	std::string getAreaName(const sf::Vector2i& coord);
	bool getWalkable(const sf::Vector2i& coord);
	void drawNodes(sf::RenderTarget& target) const;//testing

private:
	int getHeuristic(const Node* n1, const Node* n2);
	int getCost(const sf::Vector2i& offset);
	std::vector<sf::Vector2f> createPath(int startIndex, int goalIndex);
	int getNeighbourIndex(const Node * current, int neighbourOffsetIndex);
	bool isValidCoord(const sf::Vector2i& coord);

	Node** m_nodes; //1d array of nodes corresponding to 2d map
	int m_mapWidth;
	int m_mapHeight;

	const int NEIGHBOUR_COUNT = 8;
	sf::Vector2i m_neighbourOffsets[8];
	Pathfinder* that = this;
	class NodeSearchCostComparer {
	public:
		bool operator()(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
			return p1.first > p2.first;
		}
	};
};

#endif
