#include "Cloud.h"

Cloud::Cloud(sf::Sprite sprite){	
	this->sprite = sprite;
	this->sprite.setOrigin(sf::Vector2f(this->sprite.getTextureRect().width, 0));
	this->sprite.setPosition(MapLoader::isometricToOrthogonal(sf::Vector2f(0, rand() % 5000)));
	this->sprite.setColor(sf::Color(255, 255, 255, rand() % 50));
	randXVel = rand() % 5;
}

Cloud::~Cloud(){}



void Cloud::update(float dt){
		
	sprite.setPosition(sf::Vector2f(sprite.getPosition().x + 1 + randXVel, sprite.getPosition().y +1));	

	if (MapLoader::isometricToOrthogonal(sprite.getPosition()).x > MapLoader::isometricToOrthogonal(sf::Vector2f(5000,5000)).x && 
		MapLoader::isometricToOrthogonal(sprite.getPosition()).y > MapLoader::isometricToOrthogonal(sf::Vector2f(5000, 5000)).y)
	{
		alive = false;
	}
}

void Cloud::draw(sf::RenderTarget &target, sf::RenderStates states) const{	
	target.draw(sprite);		
	
}

bool Cloud::getAlive() const
{
	return alive;
}

