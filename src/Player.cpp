#include "Player.h"

Player::Player(sf::Vector2f position, sf::Texture & spriteSheet) : m_position(position) {
	m_animatedSprite = AnimatedSprite(sf::seconds(0.0333f), false, true);
	m_animatedSprite.setScale(0.2f, 0.2f);
	loadAnimations(spriteSheet);
}

void Player::loadAnimations(sf::Texture & spriteSheet) {
	 //m_JSONData;
	std::ifstream file("resources\\images\\playerTestSprite.json");
	std::string line;
	std::string jsonData;
	if (file.is_open()){
		while (std::getline(file, line)){
			jsonData.append(line);
		}
	}
	const char * json = jsonData.c_str();
	rapidjson::Document d;
	d.Parse(json);
	rapidjson::Value const & frames = d["frames"];
	sf::IntRect rect;
	int test = frames.Size();
	kickAnim.setSpriteSheet(spriteSheet);
	for (int i = 0; i < frames.Size(); i++){
		
		rect.left = frames[i]["frame"]["x"].GetInt();
		rect.top = frames[i]["frame"]["y"].GetInt();
		rect.width = frames[i]["frame"]["w"].GetInt();
		rect.height = frames[i]["frame"]["h"].GetInt();
		kickAnim.addFrame(rect);
	}
	m_animatedSprite.setOrigin(rect.width / 2.f, rect.height / 2.f);
	currentAnim = &kickAnim;
}

void Player::update(sf::Time dt){
	checkInput();
	m_position += m_velocity * dt.asSeconds();
	m_animatedSprite.play(*currentAnim);
	m_animatedSprite.setPosition(m_position);
	m_animatedSprite.update(dt);
}

void Player::checkInput(){
	bool test = sf::Joystick::isConnected(0);
	float xPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
	float yPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
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
	
}

sf::Vector2f Player::getPosition(){
	return m_position;
}