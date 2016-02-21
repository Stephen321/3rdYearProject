#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <limits>
#include "SFML\Graphics.hpp"

class Node {
public:
	Node(int x, int y, bool walkable, const std::string& area);
	bool open() const;
	bool close() const;
	int gCost() const;
	int fCost() const;
	sf::CircleShape const & getShape() const;
	Node* getPrevious() const;
	sf::Vector2f getPosition() const;
	sf::Vector2i getMapPosition() const;
	bool walkable() const;
	std::string area() const;
	void setOpen(bool open);
	void setClose(bool close);
	void setFCost(int fCost);
	void setGCost(int gCost);
	void setPrevious(Node* previous);
	void setPosition(sf::Vector2f newPosition);
	void setColour(sf::Color newColour);
	void reset();

	//overloads
	bool operator<(const Node&) const;
	bool operator==(const Node&) const;

private:
	int m_fCost;
	int m_gCost;
	bool m_open;
	bool m_close;
	bool m_walkable;
	std::string m_area;
	Node* m_prevNode;
	sf::CircleShape m_shape;
	sf::Vector2i m_mapPosition;

};
#endif
