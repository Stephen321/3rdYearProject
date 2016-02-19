#include "Pathfinding\Pathfinder.h"

Pathfinder::Pathfinder(){
	m_neighbourOffsets[0] = sf::Vector2i(-1, -1);
	m_neighbourOffsets[1] = sf::Vector2i(-1, 0);
	m_neighbourOffsets[2] = sf::Vector2i(-1, 1);
	m_neighbourOffsets[3] = sf::Vector2i(0, 1);
	m_neighbourOffsets[4] = sf::Vector2i(1, 1);
	m_neighbourOffsets[5] = sf::Vector2i(1, 0);
	m_neighbourOffsets[6] = sf::Vector2i(1, -1);
	m_neighbourOffsets[7] = sf::Vector2i(0, -1);
}

Pathfinder::~Pathfinder(){
	int size = m_mapWidth * m_mapHeight;
	//reset nodes
	for (int i = 0; i < size; i++)
		delete m_nodes[i];
	delete[] m_nodes;
}

void Pathfinder::setSize(int width, int height){
	m_mapWidth = width;
	m_mapHeight = height;
	int size = m_mapWidth * m_mapHeight;
	m_nodes = new Node*[size];
	for (int i = 0; i < size; i++)
		m_nodes[i] = 0;
}

void Pathfinder::addNode(int x, int y, bool walkable, const sf::Vector2f& position) {
	int index = x + (y * m_mapWidth);
	if (m_nodes[index] == 0){
		m_nodes[index] = new Node(x, y, walkable);
		m_nodes[index]->setPosition(position);
	}
}

int Pathfinder::getHeuristic(const Node* n1, const Node* n2){
	//return abs(n2->getMapPosition().x - n1->getMapPosition().x) + abs(n2->getMapPosition().y - n1->getMapPosition().y);
	sf::Vector2f vectorBetween = n2->getPosition() - n1->getPosition();
	float distance = sqrt((vectorBetween.x * vectorBetween.x) + (vectorBetween.y * vectorBetween.y));
	return (int)distance;
}

int Pathfinder::getCost(const sf::Vector2i& offset){
	//diagonal paths (-1,-1), (1,-1), (1,1), (-1,1) cost more
	if (offset.x != 0 && offset.y != 0)
		return 18;
	else
		return 10;
}

std::vector<sf::Vector2f> Pathfinder::createPath(int startIndex, int goalIndex){
	std::vector<sf::Vector2f> path;
	for (Node* previous = m_nodes[goalIndex]; previous->getPrevious() != 0; previous = previous->getPrevious()){
		path.push_back(previous->getPosition());
		previous->setColour(sf::Color::Magenta);
	}
	return path;
}

int Pathfinder::getNeighbourIndex(const Node * current, int neighbourOffsetIndex){
	sf::Vector2i neighbourPos = current->getMapPosition() + m_neighbourOffsets[neighbourOffsetIndex];
	if (neighbourPos.x >= 0 && neighbourPos.y >= 0 &&
		neighbourPos.x < m_mapWidth && neighbourPos.y < m_mapHeight)
		return (neighbourPos.x) + (neighbourPos.y * m_mapWidth);
	else
		return -1;
}

//see https://en.wikipedia.org/wiki/A*_search_algorithm
std::vector<sf::Vector2f> Pathfinder::findPath(const sf::Vector2i& startPos, const sf::Vector2i& goalPos){
	if (startPos.x >= 0 && startPos.y >= 0 && goalPos.x >= 0 && goalPos.y >= 0 &&
		startPos.x < m_mapWidth && startPos.y < m_mapHeight && goalPos.x < m_mapWidth && goalPos.y < m_mapHeight) {
		int size = m_mapWidth * m_mapHeight;

		int startIndex = startPos.x + (startPos.y * m_mapWidth);
		int goalIndex = goalPos.x + (goalPos.y * m_mapWidth);
		Node* start = m_nodes[startIndex];
		Node* goal = m_nodes[goalIndex];
		if (start != 0 && goal != 0){
			//reset nodes
			for (int i = 0; i < size; i++){
				if (m_nodes[i] != 0)
					m_nodes[i]->reset();
			}
			//pair is fcost,index. storing node pointers causes invalid heap
			priority_queue<std::pair<int, int>, vector<std::pair<int, int>>, NodeSearchCostComparer> openset;
			start->setGCost(0);
			start->setFCost(getHeuristic(start, goal));
			start->setOpen(true);
			openset.push(std::pair<int,int>(start->fCost(), startIndex));


			while (openset.size() != 0) {
				Node* current = m_nodes[openset.top().second];
				openset.pop();
				if (current == goal)
					return createPath(startIndex, goalIndex);
				current->setOpen(false); //remove from open
				current->setClose(true); //set as closed
				//	if (neighbour != current->getPrevious()){
				for (int i = 0; i < 8; i++) { //loop through neighbours
					int neighbourIndex = getNeighbourIndex(current, i);
					Node* neighbour = (neighbourIndex == -1) ? 0 : m_nodes[neighbourIndex];
					if (neighbour == 0 || neighbour->close() ||
						neighbour == current->getPrevious() ||
						neighbour->walkable() == false)
						continue;
					int tenativeGCost = current->gCost() + getCost(m_neighbourOffsets[i]);
					if (tenativeGCost <= neighbour->gCost()){
						neighbour->setPrevious(current);
						neighbour->setGCost(tenativeGCost);
						neighbour->setFCost(neighbour->gCost() + getHeuristic(neighbour, goal));
					}

					if (neighbour->open() == false){ //not explored
						neighbour->setOpen(true);
						neighbour->setColour(sf::Color(0, 128, 128, 255));
						openset.push(std::pair<int,int>(neighbour->fCost(), neighbourIndex));
					}
				}
			}
			if (openset.size() == 0)
				cout << "Couldn't find path." << endl;
		}
	}
	return std::vector<sf::Vector2f>();
}

void Pathfinder::drawNodes(sf::RenderTarget& target) const{
	int size = m_mapWidth * m_mapHeight;
	for (int i = 0; i < size; i++){
		if (m_nodes[i] != 0)
			target.draw(m_nodes[i]->getShape());
	}
}