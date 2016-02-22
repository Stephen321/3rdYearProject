#include "Action.h"

Action::Action(int minFrame, int maxFrame, int buttonId, int damage, int modifier, bool multiAttack, std::string animName) :
m_minFrame(minFrame),
m_maxFrame(maxFrame),
m_buttonID(buttonId),
m_damage(damage),
m_modifier(modifier),
m_multiAttack(multiAttack),
m_animName(animName){}
