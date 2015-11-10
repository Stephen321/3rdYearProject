#include "HealthBar.h"

HealthBar::HealthBar(){}

HealthBar::HealthBar(float maxHealth, sf::Vector2f position) :
m_maxHealth(maxHealth),
m_currentHealth(m_maxHealth),
m_position(position){
	float height = 15, width = 70;
	m_backgroundRect = sf::RectangleShape(sf::Vector2f(width, height));
	m_backgroundRect.setOutlineColor(sf::Color::Black);
	m_backgroundRect.setFillColor(sf::Color(128,128,128));
	m_backgroundRect.setOrigin(sf::Vector2f(width / 2.f, height));
	m_backgroundRect.setPosition(position);
	m_backgroundRect.setOutlineThickness(2.f);

	m_healthRect = sf::RectangleShape(sf::Vector2f(width, height));
	m_healthRect.setOutlineColor(sf::Color::Transparent);
	m_healthRect.setFillColor(sf::Color::Green);
	m_healthRect.setOrigin(sf::Vector2f(width / 2.f, height));
	m_healthRect.setPosition(position);
	m_healthRect.setOutlineThickness(2.f);
}

void HealthBar::update(sf::Vector2f position){
	m_backgroundRect.setPosition(position);
	m_healthRect.setPosition(position);
}

bool HealthBar::changeHealth(float change){
	bool alive = true;
	if (m_currentHealth + change <= 0){
		m_currentHealth = 0;
		alive = false;
	}
	else if (m_currentHealth + change > m_maxHealth)
		m_currentHealth = m_maxHealth;
	else
		m_currentHealth += change;
	m_healthRect.setSize(sf::Vector2f((m_currentHealth / m_maxHealth) * m_backgroundRect.getSize().x, m_backgroundRect.getSize().y));
	return alive;
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(m_backgroundRect);
	target.draw(m_healthRect);
}

void HealthBar::reset(){
	m_currentHealth = m_maxHealth;
	m_healthRect.setSize(sf::Vector2f(m_backgroundRect.getSize().x, m_backgroundRect.getSize().y));
}