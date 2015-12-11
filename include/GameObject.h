#ifndef OBJECT_H
#define OBJECT_H

#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Debug.h"
#include "GameData.h"
#include "tmx2box2d.h"
#include "CollisionFilters.h"
#include "VisibleObject.h"

class GameObject : public VisibleObject {
public:
	enum class ObjectType {
		ROCK,
		TREE
	};
	void update(sf::FloatRect viewBounds);
	bool getVisible() const;
	virtual sf::Vector2f getPosition() const;

protected:
	GameObject(b2World& world, sf::Vector2f position, CollisionFilters filter, ObjectType type);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Sprite m_sprite;
	sf::ConvexShape debugShape;
	sf::FloatRect m_bounds;
	bool m_visible;
	ObjectType m_type; 
	b2Body* m_body;
};

#endif