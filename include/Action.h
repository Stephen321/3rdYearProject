#ifndef ACTION_H
#define ACTION_H

#include <string>

class Action {
public:
	Action(int minFrame = 5, int maxFrame = 10, int buttonId = 0, int damage = 10, int modifier = 2, bool multiAttack = false, std::string animName = "");
	int getMinFrame() const { return m_minFrame; }
	int getMaxFrame() const { return m_maxFrame; }
	int getButtonId() const { return m_buttonID; }
	int getDamage() const { return m_damage; }
	int getModifier() const { return m_modifier; }
	bool getMultiAttack() const { return m_multiAttack; }
	std::string getAnimName() const { return m_animName; }
private:
	int m_minFrame;
	int m_maxFrame;
	int m_buttonID;
	int m_damage;
	int m_modifier;
	bool m_multiAttack;
	std::string m_animName;
};
#endif