#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include "Node.h"
#include <queue>

using namespace std;


// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
class Graph {
private:
    Node** m_pNodes;
    int m_maxNodes;
    int m_count;

	class NodeSearchCostComparer {
	public:
		bool operator()(Node * n1, Node * n2) {
			int f1 = n1->hCost() + n1->gCost();
			int f2 = n2->hCost() + n2->gCost();
			return f1 > f2;
		}
	};

public:           
    // Constructor and destructor functions
    Graph( int size );
    ~Graph();

    // Accessors
    Node** nodeArray() const {
       return m_pNodes;
    }

    // Public member functions.
	bool addNode(int index, sf::Vector2f position);
	void clearPrevious();
	void aStar(Node* pStart, Node* pDest, std::vector<Node *>& path){
	void setHeuristics(Node* pDest);
	void drawNodes(sf::RenderTarget& target) const;
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
Graph::Graph( int size ) : m_maxNodes( size ) {
   int i;
   m_pNodes = new Node * [m_maxNodes];
   // go through every index and clear it to null (0)
   for( i = 0; i < m_maxNodes; i++ ) {
        m_pNodes[i] = 0;
   }

   // set the node count to 0.
   m_count = 0;
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
Graph::~Graph() {
   int index;
   for( index = 0; index < m_maxNodes; index++ ) {
        if( m_pNodes[index] != 0 ) {
            delete m_pNodes[index];
        }
   }
   // Delete the actual array
   delete m_pNodes;
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
bool Graph::addNode(int index, sf::Vector2f position ) {
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( m_pNodes[index] == 0) {
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

void Graph::clearPrevious() {
	int index;
	for (index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			m_pNodes[index]->setPrevious(0);
		}
	}
}

void Graph::aStar(Node* pStart, Node* pDest, std::vector<Node *>& path){
	if (pStart != 0 && pDest != 0) {
		priority_queue<Node*, vector<Node*>, NodeSearchCostComparer> pq;
		pq.push(pStart);
		pStart->setHCost(0);
		pStart->setGCost(0);

		while (pq.size() != 0 && pq.top() != pDest) {

			for (; iter != endIter; iter++) {
				Node* child = (*iter).node();
				if (child != pq.top()->getPrevious()){
					int Gc = pq.top()->gCost() + arc.weight();
					int Fc =child->hCost() + Gc;
					int currentFc = child->hCost() + child->gCost();
					if (Fc < currentFc  || get<2>(data) == -1){  //is G(n) not set, H(n) will be set with setHeuristics()
						child->setData(NodeType(get<0>(data), Hc, Gc));
						child->setPrevious(pq.top());
					}

					if (child->marked() == false) {
						pq.push(child);
						child->setMarked(true);
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

void Graph::setHeuristics(Node* pDest){
	if (pDest != 0) {
		for (int i = 0; i < m_count; i++){
			sf::Vector2f vectorTo = pDest->getPosition() - m_pNodes[i]->getPosition();
			int Hc = (int)(sqrt((vectorTo.x * vectorTo.x) + (vectorTo.y * vectorTo.y)));
			NodeType data = m_pNodes[i]->data();
			m_pNodes[i]->setData(NodeType(get<0>(data), Hc, get<2>(data)));
		}
	}
}



//draw the nodes
void Graph::drawNodes(sf::RenderTarget& target) const{
	for (int i = 0; i < m_count; i++){
		target.draw(m_pNodes[i]->getShape());
		m_pNodes[i]->drawText(target);
	}
}


#endif
