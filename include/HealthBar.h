#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "SFML/Graphics.hpp" 

class HealthBar : public sf::Drawable{
public:
	HealthBar();
	HealthBar(float maxHealth, sf::Vector2f position);
	bool changeHealth(float change);
	void update(sf::Vector2f position);
	void reset();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RectangleShape m_backgroundRect;
	sf::RectangleShape m_healthRect;
	sf::Vector2f m_position;
	float m_maxHealth;
	float m_currentHealth;
};
#endif