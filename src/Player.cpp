#include "Player.h"

Player::Player(sf::Vector2f position, b2World& _world) :
m_position(position),
world (_world){
	m_animatedSprite = AnimatedSprite(sf::seconds(0.0333f), false, true);
	m_animatedSprite.setScale(0.2f, 0.2f);
	//m_animatedSprite.setOrigin(rect.width / 2.f, rect.height / 2.f);
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	m_anims = ptr->playerAnims;
	currentAnim = &m_anims["kick"];
	m_animatedSprite.play(*currentAnim);

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

	m_spriteOffset = sf::Vector2f(0, 6-m_animatedSprite.getGlobalBounds().height / 2.f);
	

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

void Player::update(sf::Time dt){
	checkInput();
	//m_position += m_velocity * dt.asSeconds();
	m_animatedSprite.setPosition(m_position + m_spriteOffset);
	m_animatedSprite.play(*currentAnim);
	m_animatedSprite.update(dt);
	m_animatedSprite.setOrigin(m_animatedSprite.getLocalBounds().width / 2.f, m_animatedSprite.getLocalBounds().height / 2.f);//update origin

	m_body->SetLinearVelocity(tmx::SfToBoxVec(sf::Vector2f(m_velocity)));
	m_position = tmx::BoxToSfVec(m_body->GetPosition());

	b2CircleShape* cs = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
	c.setRadius(tmx::BoxToSfFloat(cs->m_radius));
	c.setPosition(tmx::BoxToSfVec(cs->m_p) + m_position);
}

void Player::checkInput(){
	int joystick = -1;
	for (int i = 0; i < 6 && joystick == -1; i++){
		if (sf::Joystick::isConnected(i))
			joystick = i;
	}
	if (sf::Joystick::isButtonPressed(joystick, 0))
		currentAnim = &m_anims["derp"];
	if (sf::Joystick::isButtonPressed(joystick, 1))
		currentAnim = &m_anims["kick"];
	if (sf::Joystick::isButtonPressed(joystick, 2))
		currentAnim = &m_anims["win"];
	float xPos = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::X);
	float yPos = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
	if ((xPos < 10 && xPos > -10) &&
		(yPos < 10 && yPos > -10)){
		m_velocity.x = 0;
		m_velocity.y = 0;
		return;
	}
	xPos = (xPos / 100) * m_SPEED;
	yPos = (yPos / 100) * m_SPEED;

	m_velocity = sf::Vector2f(xPos, yPos);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(m_animatedSprite);
	target.draw(c);

}

sf::Vector2f Player::getPosition() const{
	return m_position;
}