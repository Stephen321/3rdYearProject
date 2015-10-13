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

#include "tmx\MapLoader.h"


////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 

#include "WaterShaderText.h"

int main()
{
	bool shader = false, showDebug = false;
	tmx::MapLoader ml("resources");
	if (shader)	ml.Load("shader_example.tmx"); else	ml.Load("isometric_grass_and_water.tmx");
	ml.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800u, 600u, 32), "3rd Year Project");
	window.setVerticalSyncEnabled(true);

	//shader testing
	sf::Shader waterEffect;
	waterEffect.loadFromMemory(waterShader, sf::Shader::Fragment);
	if (shader)	ml.SetLayerShader(0u, waterEffect);
	sf::Clock shaderClock, frameClock, deltaClock;

	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");


	sf::Text screenPosText;
	screenPosText.setFont(font);
	screenPosText.setStyle(sf::Text::Regular);
	screenPosText.setPosition(600, 10);
	screenPosText.setCharacterSize(24);

	sf::Text mapPosText;
	mapPosText.setFont(font);
	mapPosText.setStyle(sf::Text::Regular);
	mapPosText.setPosition(600, 40);
	mapPosText.setCharacterSize(24);

	// Start game loop 
	while (window.isOpen())
	{
		//----------------------------------------------------------------------------
		//debug info
		//move objects about
		/*std::vector<tmx::MapLayer>& layers = ml.GetLayers();
		for (auto& l : layers)
		{
			if (l.type == tmx::ObjectGroup)
			{
				for (auto& o : l.objects)
				{
					o.Move(0.f, 60.f * frameClock.getElapsedTime().asSeconds());
					if (o.GetPosition().y > 600.f)
					{
						o.SetPosition(o.GetPosition().x, 0.f);
					}
				}
			}
		}
		ml.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));*/

		sf::Vector2f mouseScreenPos = (sf::Vector2f)sf::Mouse::getPosition(window);
		screenPosText.setString("ScreenPos: (" + std::to_string((int)mouseScreenPos.x) + ", " +
			std::to_string((int)mouseScreenPos.y) + ")");
		sf::Vector2f mouseMapPos = ml.OrthogonalToIsometric(mouseScreenPos);
		mapPosText.setString("MapPos: (" + std::to_string((int)mouseMapPos.x) + ", " +
			std::to_string((int)mouseMapPos.y) + ")");

		//----------------------------------------------------------------------------

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
			if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::D))
				showDebug = !showDebug;
		}
		//update shader
		if (shader) waterEffect.setParameter("time", shaderClock.getElapsedTime().asSeconds());

		//prepare frame
		window.clear();

		//draw frame items
		window.draw(ml);
		if (showDebug) ml.Draw(window, tmx::MapLayer::Debug);//draw with debug info
		window.draw(screenPosText);
		window.draw(mapPosText);
		// Finally, display rendered frame on screen 
		window.display();

		window.setTitle("3rd Year Project " + std::to_string(1.f / frameClock.getElapsedTime().asSeconds()));
	} //loop back for next frame

	return EXIT_SUCCESS;
}