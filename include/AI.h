#ifndef AI_H
#define AI_H

#include "Character.h"

class AI : public Character{
public:
	AI(sf::Vector2f position, b2World& world, std::unordered_map<std::string, Animation> anims, float playSpeed, float scale);
	//protected:
private:
	void behaviour(float dt) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	mutable bool m_visible;
};
#endif