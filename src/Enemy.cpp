#include "Enemy.h"

Enemy::Enemy(b2World& world, Player* playerP, sf::Vector2f position, Pathfinder * pf) :
Character(world, CharacterType::AI, position, pf),
player(playerP),
m_target(0),
m_lastTargetTileCoord(player->getTileCoord()),
m_followPlayer(false),
m_startPos(position),
m_attackTime(1.2f),
m_visibiltyRange(450){}

void Enemy::behaviour(){
	setTarget();
	getWaypoints();
	followPath();
	handleAttack();
}


void Enemy::setTarget(){
	sf::Vector2f vB = player->getPosition() - getPosition();
	float distanceSquared = vB.x * vB.x + vB.y * vB.y;
	if (distanceSquared < (m_visibiltyRange * m_visibiltyRange)){ //player in range
		m_targetTileCoord = player->getTileCoord();
		m_followPlayer = true;
	}
	else if (m_followPlayer != false){
		m_targetTileCoord = getTileCoord(m_startPos);
		m_followPlayer = false;
	}
}

//player->getTileCoord()
void Enemy::getWaypoints(){
	if (m_lastTargetTileCoord != m_targetTileCoord){ //update path
		std::string myArea = m_pathFinder->getAreaName(getTileCoord());
		std::string targetArea = m_pathFinder->getAreaName(m_targetTileCoord);
		if (myArea == targetArea){ //same area
			vector<sf::Vector2f> newPath = m_pathFinder->findPath(getTileCoord(), m_targetTileCoord);
			if (newPath.empty() == false)
				m_waypoints = newPath;
			std::cout << "Target : x: " << m_targetTileCoord.x << " y: " << m_targetTileCoord.y << std::endl;
		}
		m_lastTargetTileCoord = m_targetTileCoord;
	}
}

void Enemy::followPath(){
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
}

void Enemy::handleAttack(){
	if (m_attacking == false && m_target != 0 && m_timer > m_attackTime){
		m_target->takeDamage(5);
		m_timer = 0;
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

void Enemy::sensorEnd(Character*){
	m_target = 0;
}

void Enemy::sensorStart(Character* c){
	m_target = c;
}