#ifndef CLOUD_H
#define CLOUD_H

#include "SFML\Graphics.hpp"
#include <memory>
#include "GameData.h"

class Cloud : public sf::Drawable
{
public:
	Cloud(sf::Sprite sprite);
	~Cloud();	
	void update(float dt);
	bool getAlive() const;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;	
private:
	sf::Sprite sprite;
	bool alive = true;
	int randXVel = 0;
	
};

#endif