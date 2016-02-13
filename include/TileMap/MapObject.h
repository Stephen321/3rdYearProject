#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "Properties.h"
#include "SFML\Graphics.hpp"
#include <vector>

class MapObject : public sf::Drawable {
public:
	enum class MapObjectType{
		Rectangle,
		//Ellipse,
		//Circle,
		//Polygon,
		Polyline//,
		//Tile
	};
	MapObject(sf::Vector2f position, sf::Vector2f size, std::string name, std::string type);
	void setObjectType(MapObjectType objectType);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void addPoint(sf::Vector2f point);
	void setObjectProperties(const PropertyMap & properties);
	MapObjectType getObjectType() const;
	sf::VertexArray getPolyPoints() const;
	int getPolyPointCount() const;
	sf::Vector2f getPosition() const;
	std::string getName() const;

private:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::string m_name;
	std::string m_type;
	PropertyMap m_properties;
	bool m_visible;
	MapObjectType m_objectType;
	sf::VertexArray m_polyPoints;
	int m_pointCount;
};

#endif