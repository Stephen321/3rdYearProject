#include "MyListener.h"

void MyListener::BeginContact(b2Contact* contact) {
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	Character* playerEntity; Character* aiEntity;
	Object* objA = static_cast<Object*>(contact->GetFixtureA()->GetBody()->GetUserData());
	if (getSensorAndOther(contact, playerEntity, aiEntity)){
		playerEntity->sensorStart(aiEntity);
	}
}

void MyListener::EndContact(b2Contact* contact) {
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	Character* sensor; Character* other;

	if (getSensorAndOther(contact, sensor, other)){
		sensor->sensorEnd(other);
	}
}

//helper function to figure out if the collision was between
//an AI and the player, and sort out which is which
bool MyListener::getSensorAndOther(b2Contact* contact, Character*& sensor, Character*& other)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//make sure only one of the fixtures was a sensor
	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();
	if (!(sensorA ^ sensorB))
		return false;

	Character* entityA = static_cast<Character*>(fixtureA->GetBody()->GetUserData());
	Character* entityB = static_cast<Character*>(fixtureB->GetBody()->GetUserData());

	if (sensorA) { //fixtureB must be an enemy 
		sensor = entityA;
		other = entityB;
	}
	else { //fixtureA must be an enemy 
		sensor = entityB;
		other = entityA;
	}
	return true;
}