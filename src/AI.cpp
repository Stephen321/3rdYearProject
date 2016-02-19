#include "AI.h"

AI::AI(b2World& world, Player* playerP, sf::Vector2f position, Pathfinder * pf) :
Character(world, CharacterType::AI, position, pf),
player(playerP),
attackTimer(0),
target(0),
m_lastPlayerTileCoord(-1,-1){}

void AI::update(sf::Time dt, sf::FloatRect viewBounds){
	behaviour();
	attackTimer += dt.asSeconds();
	Character::update(dt, viewBounds);
}

void AI::behaviour(){
	if (m_lastPlayerTileCoord != player->getTileCoord()){ //update path
		vector<sf::Vector2f> newPath = m_pathFinder->findPath(getTileCoord(), player->getTileCoord());
		if (newPath.empty() == false)
			m_waypoints = newPath;
		m_lastPlayerTileCoord = player->getTileCoord();
		std::cout << "x: " << player->getTileCoord().x << " y: " << player->getTileCoord().y << std::endl;
	}

	if (m_waypoints.empty() == false){
		sf::Vector2f vB = m_waypoints.back() - getPosition();
		float distance = std::sqrt(vB.x * vB.x + vB.y * vB.y);
		if (distance > 5)
			m_velocity = (vB / distance) * m_speed;
		else{
			m_velocity = sf::Vector2f(0, 0);
			m_waypoints.pop_back();
		}
	}
	else{
		m_velocity = sf::Vector2f(0, 0);
	}
	if (target != 0 && Debug::displayInfo)
		m_animatedSprite.setColor(sf::Color::Red);
	else if (Debug::displayInfo)
		m_animatedSprite.setColor(sf::Color::Cyan);

	if (m_attacking == false && target != 0 && attackTimer > ATTACK_TIME){
		target->takeDamage(5);
		attackTimer = 0;
		currentAnim = &m_anims["attack"];
		m_animatedSprite.play(*currentAnim);
		m_animatedSprite.setLooped(false);
		m_attacking = true;
	}
	if (m_animatedSprite.isPlaying() == false){
		currentAnim = &m_anims["idle"];
		m_animatedSprite.play(*currentAnim);
		m_animatedSprite.setLooped(true);
		m_attacking = false;
	}
}

void AI::sensorEnd(Character*){
	target = 0;
}

void AI::sensorStart(Character* c){
	target = c;
}