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

#pragma comment(lib, "zlibwapi.lib")

#include "Screen.hpp"
#include "MenuScreen.h"
#include "GameScreen.h"

int main()
{
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800u, 600u, 32), "3rd Year Project");
	window.setVerticalSyncEnabled(true);

	std::vector<Screen*> Screens;
	int screen = 0;

	//Screens preparations
	MenuScreen menuScreen;
	Screens.push_back(&menuScreen);
	GameScreen gameScreen;
	Screens.push_back(&gameScreen);

	//Main loop
	while (screen >= 0)
	{
		screen = Screens[screen]->Run(window);
	}

	return EXIT_SUCCESS;
}