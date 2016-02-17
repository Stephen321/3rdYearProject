#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <iostream>
#include <queue>
#include "Node.h"

using namespace std;

class Pathfinder {
public:           
	Pathfinder(int size);
	~Pathfinder();
	bool addNode(int index, sf::Vector2f position);
	void clearPrevious();
	void aStar(Node* pStart, Node* pDest, std::vector<Node *>& path);
	void setHeuristics(Node* pDest);
	void drawNodes(sf::RenderTarget& target) const;
	Node** Pathfinder::nodeArray() const;

private:
	Node** m_pNodes;
	int m_maxNodes;
	int m_count;

	const int NEIGHBOUR_COUNT = 8;
	sf::Vector2i m_neighbourOffsets[8];

	class NodeSearchCostComparer {
	public:
		bool operator()(Node * n1, Node * n2) {
			int f1 = n1->hCost() + n1->gCost();
			int f2 = n2->hCost() + n2->gCost();
			return f1 > f2;
		}
	};
};

#endif
