#ifndef CAMERA_H
#define CAMERA_H


#include "SFML/Graphics.hpp" 

class Camera {
private:
	sf::Vector2f m_position;
	float m_zoom;

public:
	Camera(sf::Vector2f, float);
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f);
	float getZoom();
	void setZoom(float);
};

#endif