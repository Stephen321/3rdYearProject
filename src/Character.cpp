#include "Character.h"


Character::Character(sf::Vector2f position, b2World& world, CharacterType charType) :
m_visible(false),
m_attacking(false),
m_charType(charType){
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	sndMgr = SoundManager::getInstance();
	float playSpeed = 0;
	float maxHealth = 200;
	float speed = 50;
	CollisionFilters filter;
	if (m_charType == CharacterType::PLAYER){
		m_anims = ptr->playerAnims;
		playSpeed = ptr->playerPlaySpeed;
		m_scale = ptr->playerSpriteScale;
		filter = CollisionFilters::PLAYER;
		maxHealth = 100;
		m_speed = 80;
	}
	else if (m_charType == CharacterType::AI){
		m_anims = ptr->aiAnims;
		playSpeed = ptr->aiPlaySpeed;
		m_scale = ptr->aiSpriteScale;
		filter = CollisionFilters::AI;
		maxHealth = 60;
		m_speed = 45;
	}

	m_animatedSprite = AnimatedSprite(sf::seconds(playSpeed), false, true);
	m_animatedSprite.setScale(m_scale, m_scale);
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
	circleFictureDef.filter.categoryBits = (uint16)filter;
	m_body->CreateFixture(&circleFictureDef);
	m_position = tmx::BoxToSfVec(m_body->GetPosition());

	//add sensor
	b2CircleShape circleShape2;
	circleShape2.m_radius = tmx::SfToBoxFloat(40.f);

	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &circleShape2;
	myFixtureDef.isSensor = true;
	myFixtureDef.filter.categoryBits = (uint16)filter;
	if (filter == CollisionFilters::AI)
		myFixtureDef.filter.maskBits = (uint16)CollisionFilters::PLAYER; 
	else
		myFixtureDef.filter.maskBits = (uint16)CollisionFilters::AI;
	m_body->CreateFixture(&myFixtureDef);

	m_spriteOffset = sf::Vector2f(0, 6 - m_animatedSprite.getGlobalBounds().height / 2.f);
	m_health = HealthBar(maxHealth, sf::Vector2f(0, -m_animatedSprite.getGlobalBounds().height) + m_position);


	sf::Vector2f pos = tmx::BoxToSfVec(m_body->GetPosition());
	auto test = m_body->GetFixtureList();
	b2CircleShape* cs = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetNext()->GetShape());
	float radius = tmx::BoxToSfFloat(cs->m_radius);
	c = sf::CircleShape(radius);
	c.setPosition(pos);
	c.setOrigin(radius, radius);
	c.setOutlineColor(sf::Color::Green);
	c.setOutlineThickness(-1.f);
	c.setFillColor(sf::Color::Transparent);


	b2CircleShape* cs2 = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
	radius = tmx::BoxToSfFloat(cs2->m_radius);
	sensorCircle = sf::CircleShape(radius);
	sensorCircle.setPosition(pos);
	sensorCircle.setOrigin(radius, radius);
	sensorCircle.setOutlineColor(sf::Color::Magenta);
	sensorCircle.setOutlineThickness(-1.f);
	sensorCircle.setFillColor(sf::Color::Transparent);

	m_body->SetUserData(this);
}


void Character::takeDamage(float damage){
	if (!m_health.changeHealth(-damage))
		m_health.reset();
}

void Character::startContact(){
}

void Character::endContact(){

}

void Character::setVelocity(sf::Vector2f value){
	m_velocity = value;
}

void Character::update(sf::Time dt, sf::FloatRect viewBounds){
	m_animatedSprite.setPosition(m_position + m_spriteOffset);
	m_animatedSprite.update(dt);

	//cant these 2 lines be done only when amimation changes
	m_animatedSprite.setOrigin(m_animatedSprite.getLocalBounds().width / 2.f, m_animatedSprite.getLocalBounds().height / 2.f);//update origin

	m_body->SetLinearVelocity(tmx::SfToBoxVec(sf::Vector2f(m_velocity)));
	m_position = tmx::BoxToSfVec(m_body->GetPosition());

	b2CircleShape* cs = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
	c.setPosition(tmx::BoxToSfVec(cs->m_p) + m_position);

	b2CircleShape* cs2 = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
	sensorCircle.setPosition(tmx::BoxToSfVec(cs2->m_p) + m_position);

	m_visible = m_animatedSprite.getGlobalBounds().intersects(viewBounds);

	m_health.update(sf::Vector2f(0, -m_animatedSprite.getGlobalBounds().height) + m_position);
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (m_visible){
		target.draw(m_animatedSprite);
		target.draw(m_health);
		if (Debug::displayInfo){
			target.draw(c);
			target.draw(sensorCircle);
		}
	}
}

bool Character::getVisible() const{
	return m_visible;
}