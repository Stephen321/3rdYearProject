#include "Character.h"

Character::Character(sf::Vector2f position, b2World& _world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale, float speed) :
m_position(position),
m_anims(anims),
m_visible(false),
m_SPEED(speed),
m_scale(scale),
world(_world){
	m_animatedSprite = AnimatedSprite(sf::seconds(playSpeed), false, true);
	m_animatedSprite.setScale(scale, scale);
	currentAnim = &m_anims.begin()->second;
	m_animatedSprite.play(*currentAnim);
	m_animatedSprite.setOrigin(m_animatedSprite.getLocalBounds().width / 2.f, m_animatedSprite.getLocalBounds().height / 2.f);//update origin

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	b2Vec2 b2Pos = tmx::SfToBoxVec(position);
	bodyDef.position.Set(b2Pos.x, b2Pos.y);
	bodyDef.angle = 0.f;
	m_body = world.CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_radius = tmx::SfToBoxFloat(6.f);

	b2FixtureDef circleFictureDef;
	circleFictureDef.shape = &circleShape;
	circleFictureDef.density = 1;
	circleFictureDef.restitution = 0.99f;
	m_body->CreateFixture(&circleFictureDef);
	m_position = tmx::BoxToSfVec(m_body->GetPosition());

	m_spriteOffset = sf::Vector2f(0, 6 - m_animatedSprite.getGlobalBounds().height / 2.f);


	sf::Vector2f pos = tmx::BoxToSfVec(m_body->GetPosition());
	b2CircleShape* cs = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
	float radius = tmx::BoxToSfFloat(cs->m_radius);
	c = sf::CircleShape(radius);
	c.setPosition(pos);
	c.setOrigin(radius, radius);
	c.setOutlineColor(sf::Color::Green);
	c.setOutlineThickness(-1.f);
	c.setFillColor(sf::Color::Transparent);
}

void Character::update(sf::Time dt, sf::FloatRect viewBounds){
	m_animatedSprite.setPosition(m_position + m_spriteOffset);
	m_animatedSprite.update(dt);

	//cant these 2 lines be done only when amimation changes
	m_animatedSprite.play(*currentAnim);
	m_animatedSprite.setOrigin(m_animatedSprite.getLocalBounds().width / 2.f, m_animatedSprite.getLocalBounds().height / 2.f);//update origin

	m_body->SetLinearVelocity(tmx::SfToBoxVec(sf::Vector2f(m_velocity)));
	m_position = tmx::BoxToSfVec(m_body->GetPosition());

	b2CircleShape* cs = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
	c.setRadius(tmx::BoxToSfFloat(cs->m_radius));
	c.setPosition(tmx::BoxToSfVec(cs->m_p) + m_position);

	m_visible = m_animatedSprite.getGlobalBounds().intersects(viewBounds);
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (m_visible){
		target.draw(m_animatedSprite);
		target.draw(c);
	}
}

sf::Vector2f Character::getPosition() const{
	return m_position;
}

bool Character::getVisible() const{
	return m_visible;
}