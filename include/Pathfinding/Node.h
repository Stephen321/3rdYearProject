#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "SFML\Graphics.hpp"

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
class Node {
private:    
// typedef the classes to make our lives easier.
// -------------------------------------------------------
// Description: data inside the node
// -------------------------------------------------------
	int m_hCost;
	int m_gCost;

// -------------------------------------------------------
// Description: This remembers if the node is marked.
// -------------------------------------------------------
    bool m_marked;

	Node* m_prevNode; 

	sf::CircleShape m_shape;

	const int RADIUS = 25;

public:
    // Accessor functions
    bool marked() const {
        return m_marked;
    }

	int hCost() const {
		return m_hCost;
	}
	int gCost() const {
		return m_gCost;
	}
    int fCost() const {
		return m_hCost + m_gCost;
    }
	void setHCost(int hCost) {
		m_hCost = hCost;
	}
	void setGCost(int gCost) {
		m_gCost = gCost;
	}

	void setPrevious(Node* previous) {
		m_prevNode = previous;
	}

	void setColour(sf::Color newColour) {
		m_shape.setFillColor(newColour);
	}

	void setPosition(sf::Vector2f newPosition){
		m_shape.setPosition(newPosition);
	}

	sf::Vector2f getPosition(){
		return m_shape.getPosition();
	}

	sf::CircleShape const & getShape() const{
		return m_shape;
	}

	Node* getPrevious() {
		return m_prevNode;
	}
	Node();
};


Node::Node() : m_hCost(-1), m_gCost(-1){
	m_prevNode = 0;
	m_shape = sf::CircleShape(RADIUS);
	m_shape.setOrigin(RADIUS, RADIUS);
}
#endif
