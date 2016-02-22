#include "GameObject.h"

GameObject::GameObject(b2World& world, sf::Vector2f position, ObjectType type) :
m_type(type){
	std::shared_ptr<GameData> ptr = GameData::getInstance();

	if (type == ObjectType::ROCK)
		m_sprite = sf::Sprite(ptr->rockTexture);
	else if (type == ObjectType::BUSH)
		m_sprite = sf::Sprite(ptr->bushTexture);
	else if (type == ObjectType::TREE)
		m_sprite = sf::Sprite(ptr->treeTexture);
	else if (type == ObjectType::TALLGRASS)
		m_sprite = sf::Sprite(ptr->grassBladeTextures[0]);

	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2.f, m_sprite.getTextureRect().height - 16);
	m_position = position;

	//test
	centreTest = sf::CircleShape(5);
	centreTest.setOrigin(5, 5);
	centreTest.setPosition(position);
	centreTest.setOutlineColor(sf::Color::Blue);
	centreTest.setFillColor(sf::Color::Transparent);
	centreTest.setOutlineThickness(2);

	m_bounds.left = m_position.x - m_sprite.getOrigin().x;
	m_bounds.top = m_position.y - m_sprite.getOrigin().y; 
	m_bounds.width = m_sprite.getGlobalBounds().width;
	m_bounds.height = m_sprite.getGlobalBounds().height;
	m_sprite.setPosition(m_position);

	//test
	boundsTest = sf::RectangleShape();
	boundsTest.setSize(sf::Vector2f(m_bounds.width, m_bounds.height));
	boundsTest.setFillColor(sf::Color::Transparent);
	boundsTest.setOutlineColor(sf::Color::Blue);
	boundsTest.setOutlineThickness(2.f);
	boundsTest.setPosition(m_bounds.left, m_bounds.top);
}

void GameObject::update(sf::FloatRect viewBounds){
	m_visible = m_bounds.intersects(viewBounds);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (m_visible){
		target.draw(m_sprite);
		//target.draw(centreTest);
		//target.draw(boundsTest);
	}
}

bool GameObject::getVisible() const{
	return m_visible;
}

sf::Vector2f GameObject::getPosition() const{
	return m_position;
}