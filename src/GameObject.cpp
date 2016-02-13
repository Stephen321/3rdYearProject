#include "GameObject.h"

GameObject::GameObject(b2World& world, sf::Vector2f position, CollisionFilters filter, ObjectType type) :
m_type(type){
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	m_sprite = sf::Sprite(ptr->rockTexture);
	m_sprite.setOrigin(ptr->rockTexture.getSize().x / 2.f, ptr->rockTexture.getSize().y / 2.f);

	m_position = position;
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	b2Vec2 b2Pos = SfToBoxVec(position);
	bodyDef.position.Set(b2Pos.x, b2Pos.y);
	m_body = world.CreateBody(&bodyDef);

	b2PolygonShape polyShape;
	polyShape.SetAsBox(SfToBoxFloat(m_sprite.getOrigin().x), SfToBoxFloat(m_sprite.getOrigin().y / 4.f));
	polyShape.m_centroid.y = -SfToBoxFloat(m_sprite.getOrigin().y - 35);
	debugShape = sf::ConvexShape(4);

	for (int i = 0; i < 4; i++){
		polyShape.m_vertices[i].y += polyShape.m_centroid.y;
		debugShape.setPoint(i, m_position + BoxToSfVec(polyShape.GetVertex(i)));
	}
	
	b2FixtureDef polyFictureDef;
	polyFictureDef.shape = &polyShape;
	polyFictureDef.density = 1;
	polyFictureDef.restitution = 0.99f;
	polyFictureDef.filter.categoryBits = (uint16)filter;
	m_body->CreateFixture(&polyFictureDef);
	m_position = BoxToSfVec(m_body->GetPosition());

	m_bounds.left = m_position.x - m_sprite.getOrigin().x;
	m_bounds.top = m_position.y - m_sprite.getOrigin().y; 
	m_bounds.width = m_sprite.getOrigin().x * 2;
	m_bounds.height = m_sprite.getOrigin().y * 2;

	debugShape.setFillColor(sf::Color::Transparent);
	debugShape.setOutlineColor(sf::Color(30, 70, 10));
	debugShape.setOutlineThickness(2.f);
	m_sprite.setPosition(m_position);
	m_body->SetUserData(this);
}

void GameObject::update(sf::FloatRect viewBounds){
	m_visible = m_bounds.intersects(viewBounds);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (m_visible){
		target.draw(m_sprite);
		if (Debug::displayInfo){
			target.draw(debugShape);
		}
	}
}

bool GameObject::getVisible() const{
	return m_visible;
}


sf::Vector2f GameObject::getPosition() const{
	b2Fixture* f = m_body->GetFixtureList();
	b2PolygonShape* ps = (b2PolygonShape*)f->GetShape();
	return m_position + BoxToSfVec(ps->m_centroid);
}