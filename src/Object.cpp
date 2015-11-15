#include "Object.h"

Object::Object(b2World& world, sf::Vector2f position, Object::ObjectType type) :
m_type(type){
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	m_sprite = sf::Sprite(ptr->rockTexture);
	m_sprite.setOrigin(ptr->rockTexture.getSize().x / 2.f, ptr->rockTexture.getSize().y / 2.f);

	m_position = position;
	/*b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	b2Vec2 b2Pos = tmx::SfToBoxVec(position);
	bodyDef.position.Set(b2Pos.x, b2Pos.y);
	m_body = world.CreateBody(&bodyDef);

	b2PolygonShape polyShape;
	polyShape.SetAsBox(m_sprite.getOrigin().x, m_sprite.getOrigin().y);

	b2FixtureDef polyFictureDef;
	polyFictureDef.shape = &polyShape;
	polyFictureDef.density = 1;
	polyFictureDef.restitution = 0.99f;
	m_body->CreateFixture(&polyFictureDef);
	m_position = tmx::BoxToSfVec(m_body->GetPosition());*/

	m_bounds.left = m_position.x - m_sprite.getOrigin().x;
	m_bounds.top = m_position.y - m_sprite.getOrigin().y; 
	m_bounds.width = m_sprite.getOrigin().x * 2;
	m_bounds.height = m_sprite.getOrigin().y * 2;

	debugShape = sf::ConvexShape(4);
	for (int i = 0; i < 4; i++){
		//debugShape.setPoint(i, tmx::BoxToSfVec(polyShape.GetVertex(i)));
	}
}

void Object::update(sf::FloatRect viewBounds){
	m_visible = m_bounds.intersects(viewBounds);
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (m_visible){
		target.draw(m_sprite);
		if (Debug::displayInfo){
			target.draw(debugShape);
		}
	}
}