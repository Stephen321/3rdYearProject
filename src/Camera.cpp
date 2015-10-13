#include "Camera.h"
//void Matrix Transform
//{
//    get { return Matrix.CreateTranslation(-Pos.X, -Pos.Y, 0) * Matrix.CreateScale(zoom, zoom, 0) * Matrix.CreateTranslation(Origin); }
//}
//
//void Vector3 Origin
//{
//    get { return new Vector3(graphicDevice.Viewport.Width * 0.5f, graphicDevice.Viewport.Height * 0.5f, 0); }
//}
Camera::Camera(sf::Vector2f position, float zoom) :
m_position(position),
m_zoom(zoom)
{}
sf::Vector2f Camera::getPosition(){
	return m_position;
}
void Camera::setPosition(sf::Vector2f value){
	m_position = value;
}
float Camera::getZoom(){
	return m_zoom;
}
void Camera::setZoom(float value){
	m_zoom = value;
}
