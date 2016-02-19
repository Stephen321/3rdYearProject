#include "Pathfinding\Node.h"

Node::Node(int x, int y, bool walkable) :
	m_mapPosition(x, y),
	m_walkable(walkable),
	m_open(false),
	m_close(false),
	m_fCost(std::numeric_limits<int>::max()),
	m_gCost(std::numeric_limits<int>::max()),
	m_shape(5),
	m_prevNode(0){
	m_shape.setOrigin(5, 5);
	if (m_walkable)
		m_shape.setFillColor(sf::Color::Blue);
	else
		m_shape.setFillColor(sf::Color::Red);
}

bool Node::open() const {
	return m_open;
}

bool Node::close() const {
	return m_close;
}

int Node::gCost() const {
	return m_gCost;
}

int Node::fCost() const {
	return m_fCost;
}

Node* Node::getPrevious() const {
	return m_prevNode;
}

sf::CircleShape const & Node::getShape() const{
	return m_shape;
}

sf::Vector2i Node::getMapPosition() const{
	return m_mapPosition;
}

bool Node::walkable() const{
	return m_walkable;
}

void Node::setOpen(bool open){
	m_open = open;
}

void Node::setClose(bool close){
	m_close = close;
}

void Node::setFCost(int fCost) {
	m_fCost = fCost;
}
void Node::setGCost(int gCost) {
	m_gCost = gCost;
}

void Node::setPrevious(Node* previous) {
	m_prevNode = previous;
}

void Node::setPosition(sf::Vector2f newPosition) {
	m_shape.setPosition(newPosition);
}

void Node::setColour(sf::Color newColour) {
	m_shape.setFillColor(newColour);
}

sf::Vector2f Node::getPosition() const {
	return m_shape.getPosition();
}

void Node::reset(){
	m_prevNode = 0;
	m_fCost = std::numeric_limits<int>::max();
	m_gCost = std::numeric_limits<int>::max();
	m_close = false;
	m_open = false;
	if (m_walkable)
		setColour(sf::Color::Blue);
	else
		setColour(sf::Color::Red);
}


bool Node::operator<(const Node& n) const{
	return n.fCost() < this->m_fCost;
}

bool Node::operator==(const Node& n) const{
	return n.fCost() == this->m_fCost;
}