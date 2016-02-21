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
		m_sprite = sf::Sprite(ptr->tallGrassTexture);

	m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2.f, m_sprite.getTexture()->getSize().y / 2.f);
	m_position = position;

	m_bounds.left = m_position.x - m_sprite.getOrigin().x;
	m_bounds.top = m_position.y - m_sprite.getOrigin().y; 
	m_bounds.width = m_sprite.getOrigin().x * 2;
	m_bounds.height = m_sprite.getOrigin().y * 2;
	m_sprite.setPosition(m_position);
}

void GameObject::update(sf::FloatRect viewBounds){
	m_visible = m_bounds.intersects(viewBounds);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (m_visible){
		target.draw(m_sprite);
	}
}

bool GameObject::getVisible() const{
	return m_visible;
}

sf::Vector2f GameObject::getPosition() const{
	return m_position;
}