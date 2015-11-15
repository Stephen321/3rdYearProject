#ifndef OBJECT_H
#define OBJECT_H

#include "SFML\Graphics.hpp"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Debug.h"
#include "GameData.h"
#include "tmx2box2d.h"

class Object : public sf::Drawable {
public:
	enum class ObjectType {
		ROCK,
		TREE
	};
	Object(b2World& world, sf::Vector2f position, Object::ObjectType type = Object::ObjectType::ROCK);
	void update(sf::FloatRect viewBounds);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Sprite m_sprite;
	sf::Vector2f m_position;
	sf::ConvexShape debugShape;
	sf::FloatRect m_bounds;
	bool m_visible;
	ObjectType m_type; 
	b2Body* m_body;
};

#endif