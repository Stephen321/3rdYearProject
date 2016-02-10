#include "Character.h"


Character::Character(b2World& world, CharacterType charType, sf::Vector2f position) :
m_visible(false),
m_attacking(false),
m_alive(true){
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	sndMgr = SoundManager::getInstance();

	GameData::CharInfo* info;
	if (charType == CharacterType::PLAYER)
		info = &ptr->playerInfo;
	else if (charType == CharacterType::AI)
		info = &ptr->aiInfo;
	else
		return;

	float playSpeed = info->playSpeed;
	float maxHealth = info->maxHealth;
	m_anims = info->anims;
	m_scale = info->spriteScale; 
	CollisionFilters filterCategory = info->filterCategory;
	CollisionFilters filterMask = info->filterMask;
	m_speed = info->maxSpeed;

	m_animatedSprite = AnimatedSprite(sf::seconds(playSpeed), false, true);
	m_animatedSprite.setScale(m_scale, m_scale);
	currentAnim = &m_anims.begin()->second;
	m_animatedSprite.play(*currentAnim);
	m_animatedSprite.setOrigin(m_animatedSprite.getLocalBounds().width / 2.f, m_animatedSprite.getLocalBounds().height / 2.f);//update origin
	m_health = HealthBar(maxHealth, sf::Vector2f(0, -m_animatedSprite.getGlobalBounds().height) + position);

	setUpBox2D(world, tmx::SfToBoxVec(position), filterCategory, filterMask);
}

void Character::setUpBox2D(b2World& world, b2Vec2 position, CollisionFilters filterCategory, CollisionFilters filterMask){
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.angle = 0.f;
	m_body = world.CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_radius = tmx::SfToBoxFloat(6.f);

	b2FixtureDef circleFictureDef;
	circleFictureDef.shape = &circleShape;
	circleFictureDef.density = 1;
	circleFictureDef.restitution = 0.99f;
	circleFictureDef.filter.categoryBits = (uint16)filterCategory;
	m_body->CreateFixture(&circleFictureDef);

	//add sensor
	b2CircleShape circleShape2;
	circleShape2.m_radius = tmx::SfToBoxFloat(40.f);

	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &circleShape2;
	myFixtureDef.isSensor = true;
	myFixtureDef.filter.categoryBits = (uint16)filterCategory;
	myFixtureDef.filter.maskBits = (uint16)filterMask;
	m_body->CreateFixture(&myFixtureDef);

	m_spriteOffset = sf::Vector2f(0, 6 - m_animatedSprite.getGlobalBounds().height / 2.f);


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
	m_alive = m_health.changeHealth(-damage);
}

void Character::startContact(){
}

void Character::endContact(){

}

void Character::reset(sf::Vector2f resetPos){
	m_health.reset();
	m_alive = true;
	m_position = resetPos;
	m_body->SetTransform(tmx::SfToBoxVec(m_position), m_body->GetAngle());
}

bool Character::getAlive() const{
	return m_alive;
}

void Character::setVelocity(sf::Vector2f value){
	m_velocity = value;
}

void Character::update(sf::Time dt, sf::FloatRect viewBounds){
	//cant these 2 lines be done only when amimation changes
	m_animatedSprite.setOrigin(m_animatedSprite.getLocalBounds().width / 2.f, m_animatedSprite.getLocalBounds().height / 2.f);//update origin

	m_body->SetLinearVelocity(tmx::SfToBoxVec(sf::Vector2f(m_velocity)));

	b2CircleShape* cs = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
	c.setPosition(tmx::BoxToSfVec(cs->m_p) + getPosition());

	b2CircleShape* cs2 = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
	sensorCircle.setPosition(tmx::BoxToSfVec(cs2->m_p) + getPosition());

	m_visible = m_animatedSprite.getGlobalBounds().intersects(viewBounds);

	m_health.update(sf::Vector2f(0, -m_animatedSprite.getGlobalBounds().height) + getPosition());
	m_position = getPosition();
	m_animatedSprite.setPosition(getPosition() + m_spriteOffset);
	m_animatedSprite.update(dt);
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

sf::Vector2f Character::getPosition(){
	return tmx::BoxToSfVec(m_body->GetPosition());
}

void Character::setPosition(sf::Vector2f position){
	m_body->SetTransform(tmx::SfToBoxVec(position), m_body->GetAngle());
}