#ifndef MYLISTENER_H
#define MYLISTENER_H

#include "SFML/Graphics.hpp" 
#include "Box2D\Dynamics\b2WorldCallbacks.h"
#include "Box2D\Dynamics\Contacts\b2Contact.h"
#include "Player.h"
#include "AI.h"
#include "Object.h"

class MyListener : public b2ContactListener{
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);

private:
	bool getSensorAndOther(b2Contact* contact, Character*& sensor, Character*& other);
};
#endif