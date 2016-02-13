#include "TileMap\MapObject.h"

MapObject::MapObject(sf::Vector2f position, sf::Vector2f size, std::string name, std::string type) :
m_position(position),
m_size(size),
m_name(name),
m_type(type),
m_visible(true),
m_pointCount(0){
	m_polyPoints.setPrimitiveType(sf::PrimitiveType::Lines);
}

void MapObject::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(m_polyPoints);
}

void MapObject::setObjectType(MapObjectType objectType){
	m_objectType = objectType; 
}


void MapObject::addPoint(sf::Vector2f point){
	m_polyPoints.append(m_position + point);
	m_pointCount++;
}

void MapObject::setObjectProperties(const PropertyMap & properties){
	m_properties = properties;
}

MapObject::MapObjectType MapObject::getObjectType() const{
	return m_objectType;
}

sf::VertexArray MapObject::getPolyPoints() const{
	return m_polyPoints;
}

int MapObject::getPolyPointCount() const{
	return m_pointCount;
}

sf::Vector2f MapObject::getPosition() const{
	return m_position;
}

std::string MapObject::getName() const{
	return m_name;
}