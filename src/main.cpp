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

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>

#include "Screens.h"
#include "GameLoader.h"

#include "fmod.hpp"
#include "fmod_errors.h"

#include <string.h>
#include <malloc.h>
#include <stdio.h>

int main()
{

	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800u, 600u, 32), "3rd Year Project");
	//window.setVerticalSyncEnabled(true);

	//sound system
	FMOD::System * soundSystem;
	FMOD::System_Create(&soundSystem);
	FMOD::Channel * channel;
	FMOD::Sound * test;
	FMOD_RESULT result = FMOD_OK;
	result = soundSystem->init(100, FMOD_INIT_NORMAL, 0);
	result = soundSystem->createSound("resources/sounds/birdTweet_1.mp3", FMOD_DEFAULT, 0, &test);
	if (result != FMOD_OK){
		std::cout << "FMOD ERROR?!?!: " << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
	
	result = soundSystem->playSound(FMOD_CHANNEL_FREE, test, false, &channel);
	if (result != FMOD_OK){
		std::cout << "FMOD ERROR?!?!: " << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}

	//loading 
	std::shared_ptr<GameData> m_ptr = GameData::getInstance();
	GameLoader gl("resources/", soundSystem);

	std::vector<Screen*> Screens;
	int screen = 0;

	//Screens preparations
	MenuScreen menuScreen;
	Screens.push_back(&menuScreen);
	GameScreen gameScreen(soundSystem);
	Screens.push_back(&gameScreen);
	OptionsScreen optionsScreen;
	Screens.push_back(&optionsScreen);
	CreditsScreen creditsScreen;
	Screens.push_back(&creditsScreen);

	//Main loop
	while (screen >= 0)
	{
		screen = Screens[screen]->Run(window);
	}

	return EXIT_SUCCESS;
} 