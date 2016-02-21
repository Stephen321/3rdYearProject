#ifndef OBJECT_H
#define OBJECT_H

#include "GameData.h"
#include "VisibleObject.h"

class GameObject : public VisibleObject {
public:
	enum class ObjectType {
		ROCK,
		TREE,
		BUSH,
		TALLGRASS
	};
	virtual void update(sf::FloatRect viewBounds);
	bool getVisible() const;
	virtual sf::Vector2f getPosition() const;

protected:
	GameObject(b2World& world, sf::Vector2f position, ObjectType type);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::CircleShape centreTest;
	sf::RectangleShape boundsTest;
	sf::Sprite m_sprite;
	sf::FloatRect m_bounds;
	bool m_visible;
	ObjectType m_type; 
};

#endif