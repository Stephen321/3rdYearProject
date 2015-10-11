//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "spine/spine-sfml.h"


#include "SFML/Graphics.hpp" 
#include <SFML/Window/Mouse.hpp>
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace spine;
#include <stdio.h>


////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 

void callback(AnimationState* state, int trackIndex, EventType type, Event* event, int loopCount) {
	TrackEntry* entry = AnimationState_getCurrent(state, trackIndex);
	const char* animationName = (entry && entry->animation) ? entry->animation->name : 0;

	switch (type) {
	case ANIMATION_START:
		printf("%d start: %s\n", trackIndex, animationName);
		break;
	case ANIMATION_END:
		printf("%d end: %s\n", trackIndex, animationName);
		break;
	case ANIMATION_COMPLETE:
		printf("%d complete: %s, %d\n", trackIndex, animationName, loopCount);
		break;
	case ANIMATION_EVENT:
		printf("%d event: %s, %s: %d, %f, %s\n", trackIndex, animationName, event->data->name, event->intValue, event->floatValue,
			event->stringValue);
		break;
	}
	fflush(stdout);
}

void testPlayer() {
	// Load atlas, skeleton, and animations.
	Atlas* atlas = Atlas_createFromFile("data/testPlayer.atlas", 0);
	SkeletonJson* json = SkeletonJson_create(atlas);
	json->scale = 0.6f;
	SkeletonData *skeletonData = SkeletonJson_readSkeletonDataFile(json, "data/testPlayer.json");
	if (!skeletonData) {
		printf("%s\n", json->error);
		exit(0);
	}
	SkeletonJson_dispose(json);
	SkeletonBounds* bounds = SkeletonBounds_create();

	// Configure mixing.
	AnimationStateData* stateData = AnimationStateData_create(skeletonData);
	AnimationStateData_setMixByName(stateData, "walk", "jump", 0.2f);
	AnimationStateData_setMixByName(stateData, "jump", "run", 0.2f);

	SkeletonDrawable* drawable = new SkeletonDrawable(skeletonData, stateData);
	drawable->timeScale = 1;

	Skeleton* skeleton = drawable->skeleton;
	skeleton->flipX = false;
	skeleton->flipY = false;
	Skeleton_setToSetupPose(skeleton);

	skeleton->x = 320;
	skeleton->y = 460;
	Skeleton_updateWorldTransform(skeleton);

	Slot* headSlot = Skeleton_findSlot(skeleton, "head");

	drawable->state->listener = callback;
	if (false) {
		AnimationState_setAnimationByName(drawable->state, 0, "test", true);
	}
	else {
		AnimationState_setAnimationByName(drawable->state, 0, "walk", true);
		AnimationState_addAnimationByName(drawable->state, 0, "jump", false, 3);
		AnimationState_addAnimationByName(drawable->state, 0, "run", true, 0);
	}

	sf::RenderWindow window(sf::VideoMode(640, 480), "testPlayer");
	window.setFramerateLimit(60);
	sf::Event event;
	sf::Clock deltaClock;
	while (window.isOpen()) {
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed) window.close();

		float delta = deltaClock.getElapsedTime().asSeconds();
		deltaClock.restart();

		SkeletonBounds_update(bounds, skeleton, true);
		sf::Vector2i position = sf::Mouse::getPosition(window);
		if (SkeletonBounds_containsPoint(bounds, position.x, position.y)) {
			headSlot->g = 0;
			headSlot->b = 0;
		}
		else {
			headSlot->g = 1;
			headSlot->b = 1;
		}

		drawable->update(delta);

		window.clear();
		window.draw(*drawable);
		window.display();
	}

	SkeletonData_dispose(skeletonData);
	SkeletonBounds_dispose(bounds);
	Atlas_dispose(atlas);
}

int main()
{
	
	
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "3rd Year Project");

	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	//create a formatted text string
	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setStyle(sf::Text::Regular);
	fpsText.setPosition(20, 10);
	fpsText.setCharacterSize(32);

	//stats
	sf::Clock clock;
	float fps = 0;
	float frames = 0;
	clock.restart();

	// Start game loop 
	while (window.isOpen())
	{
		//calculate frames per second
		frames += 1;
		if (clock.getElapsedTime().asSeconds() >= 1){
			fps = (int)frames;
			frames = 0;
			clock.restart();
			fpsText.setString("Fps: " + std::to_string(fps));
		}

		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		//prepare frame
		window.clear();

		//draw frame items
		window.draw(fpsText);

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}