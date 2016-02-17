#include "Pathfinding\Pathfinder.h"

Pathfinder::Pathfinder(int size) : m_maxNodes(size) {
	int i;
	m_pNodes = new Node *[m_maxNodes];
	// go through every index and clear it to null (0)
	for (i = 0; i < m_maxNodes; i++) {
		m_pNodes[i] = 0;
	}

	// set the node count to 0.
	m_count = 0;

	m_neighbourOffsets[0] = sf::Vector2i(-1, -1);
	m_neighbourOffsets[1] = sf::Vector2i(-1, 0);
	m_neighbourOffsets[2] = sf::Vector2i(-1, 1);
	m_neighbourOffsets[3] = sf::Vector2i(0, 1);
	m_neighbourOffsets[4] = sf::Vector2i(1, 1);
	m_neighbourOffsets[5] = sf::Vector2i(1, 0);
	m_neighbourOffsets[6] = sf::Vector2i(1, -1);
	m_neighbourOffsets[7] = sf::Vector2i(0, -1);
}

Pathfinder::~Pathfinder() {
	int index;
	for (index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			delete m_pNodes[index];
		}
	}
	// Delete the actual array
	delete m_pNodes;
}

Node** Pathfinder::nodeArray() const {
	return m_pNodes;
}

bool Pathfinder::addNode(int index, sf::Vector2f position) {
	bool nodeNotPresent = false;
	// find out if a node does not exist at that index.
	if (m_pNodes[index] == 0) {
		nodeNotPresent = true;
		// create a new node, put the data in it, and unmark it.
		m_pNodes[index] = new Node();
		m_pNodes[index]->setPosition(position);
		m_pNodes[index]->setColour(sf::Color::Blue);

		// increase the count and return success.
		m_count++;
	}

	return nodeNotPresent;
}

void Pathfinder::clearPrevious() {
	int index;
	for (index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			m_pNodes[index]->setPrevious(0);
		}
	}
}

void Pathfinder::aStar(Node* pStart, Node* pDest, std::vector<Node *>& path){
	if (pStart != 0 && pDest != 0) {
		priority_queue<Node*, vector<Node*>, NodeSearchCostComparer> pq;
		pq.push(pStart);
		pStart->setHCost(0);
		pStart->setGCost(0);

		while (pq.size() != 0 && pq.top() != pDest) {
			for (int i = 0; i < 8; i++) {
				Node* child = 0;//(*iter).node();
				if (child != pq.top()->getPrevious()){
					int Hc = child->hCost();
					int Gc = pq.top()->gCost() + 10;//  +arc.weight();
					int Fc = Hc + Gc;
					if (Fc < child->fCost() || child->gCost() == -1){  //is G(n) not set, H(n) should be always set with setHeuristics()
						child->setHCost(Hc);
						child->setGCost(Gc);
						child->setPrevious(pq.top());
					}

					if (child->marked() == false) {
						pq.push(child);
						child->setColour(sf::Color(0, 128, 128, 255));
					}
				}
			}
			pq.pop();
		}

		if (pq.size() != 0 && pq.top() == pDest){
			for (Node* previous = pDest; previous->getPrevious() != 0; previous = previous->getPrevious()){
				path.push_back(previous);
				previous->setColour(sf::Color::Magenta);
			}
			path.push_back(pStart);
			std::reverse(path.begin(), path.end());
		}
		else if (pq.size() == 0)
			cout << "Couldn't find path." << endl;
	}
}

void Pathfinder::setHeuristics(Node* pDest){
	if (pDest != 0) {
		for (int i = 0; i < m_count; i++){
			sf::Vector2f vectorTo = pDest->getPosition() - m_pNodes[i]->getPosition();
			int Hc = (int)(sqrt((vectorTo.x * vectorTo.x) + (vectorTo.y * vectorTo.y)));
			m_pNodes[i]->setHCost(Hc);
		}
	}
}



//draw the nodes
void Pathfinder::drawNodes(sf::RenderTarget& target) const{
	for (int i = 0; i < m_count; i++){
		target.draw(m_pNodes[i]->getShape());
	}
}