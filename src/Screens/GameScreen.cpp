#include "Screens\GameScreen.h"

GameScreen::GameScreen(void)
{
	movement_step = 5;
	posx = 320;
	posy = 240;
	//Setting sprite
	Rectangle.setFillColor(sf::Color(255, 255, 255, 150));
	Rectangle.setSize({ 10.f, 10.f });
}

int GameScreen::Run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;

	bool shader = false;
	bool showDebug = false;

	tmx::MapLoader ml("resources");
	if (shader)	ml.Load("shader_example.tmx"); else	ml.Load("isometric_grass_and_water.tmx");
	ml.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

	Player player(ml.IsometricToOrthogonal(sf::Vector2f(320, 400)));

	sf::View view;
	view.reset(sf::FloatRect(0, 0, 443, 333));
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	window.setView(view);
	sf::Vector2f followPosition = view.getCenter();
	sf::Clock shaderClock, frameClock, deltaClock;

	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");


	sf::Text screenPosText;
	screenPosText.setFont(font);
	screenPosText.setStyle(sf::Text::Regular);
	screenPosText.setCharacterSize(20);

	sf::Text mapPosText;
	mapPosText.setFont(font);
	mapPosText.setStyle(sf::Text::Regular);
	mapPosText.setCharacterSize(20);

	sf::Text worldPosText;
	worldPosText.setFont(font);
	worldPosText.setStyle(sf::Text::Regular);
	worldPosText.setCharacterSize(20);

	sf::Text debugText1;
	debugText1.setFont(font);
	debugText1.setStyle(sf::Text::Regular);
	debugText1.setCharacterSize(20);

	while (Running)
	{
		//debug info
		//move objects about
		std::vector<tmx::MapLayer>& layers = ml.GetLayers();
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
		ml.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

		window.setView(view); //need to change view back to mouse pos info is correct
		sf::Vector2f mouseScreenPos = (sf::Vector2f)sf::Mouse::getPosition(window);
		screenPosText.setString("ScreenPos: (" + std::to_string((int)mouseScreenPos.x) + ", " +
		std::to_string((int)mouseScreenPos.y) + ")");
		sf::Vector2f mouseMapPos = ml.OrthogonalToIsometric(window.mapPixelToCoords((sf::Vector2i)mouseScreenPos));
		mapPosText.setString("MapPos: (" + std::to_string((int)mouseMapPos.x) + ", " +
		std::to_string((int)mouseMapPos.y) + ")");
		sf::Vector2f mouseWorldPos = window.mapPixelToCoords((sf::Vector2i)mouseScreenPos);
		worldPosText.setString("WorldPos: (" + std::to_string((int)mouseWorldPos.x) + ", " +
		std::to_string((int)mouseWorldPos.y) + ")");

		debugText1.setString("PlayerPos: (" + std::to_string((int)player.getPosition().x) + ", " +
		std::to_string((int)player.getPosition().y) + ")");

		//Verifying events
		while (window.pollEvent(Event))
		{
			// Window closed
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}

			if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::D))
				showDebug = !showDebug;

			//Key pressed
			if (Event.type == sf::Event::KeyPressed)
			{
				switch (Event.key.code)
				{
				case sf::Keyboard::Escape:
					return (0);
					break;
				case sf::Keyboard::Up:
					posy -= movement_step;
					break;
				case sf::Keyboard::Down:
					posy += movement_step;
					break;
				case sf::Keyboard::Left:
					posx -= movement_step;
					break;
				case sf::Keyboard::Right:
					posx += movement_step;
					break;
				default:
					break;
				}
			}
		}

		//Updating
		if (posx>630)
			posx = 630;
		if (posx<0)
			posx = 0;
		if (posy>470)
			posy = 470;
		if (posy<0)
			posy = 0;
		Rectangle.setPosition({ posx, posy });
		
		//update stuff
		player.update(frameClock.restart());

		//Clearing screen
		window.clear(sf::Color(0, 0, 0, 0));
		//Drawing
	
		view.setCenter(player.getPosition());
		window.setView(view);

		window.draw(ml);
		window.draw(player);
		

		//debug
		window.setView(window.getDefaultView());
		sf::Vector2f screenTxtPos = window.getView().getCenter() + sf::Vector2f(((window.getView().getSize().x / 2) - 210), (-window.getView().getSize().y / 2) + 0);
		sf::Vector2f mapTxtPos = window.getView().getCenter() + sf::Vector2f(((window.getView().getSize().x / 2) - 175), (-window.getView().getSize().y / 2) + 40);
		sf::Vector2f worldTxtPos = window.getView().getCenter() + sf::Vector2f(((window.getView().getSize().x / 2) - 195), (-window.getView().getSize().y / 2) + 80);
		sf::Vector2f debugPos1 = window.getView().getCenter() + sf::Vector2f(((window.getView().getSize().x / 2) - 195), (-window.getView().getSize().y / 2) + 120);
		screenPosText.setPosition(screenTxtPos);
		mapPosText.setPosition(mapTxtPos);
		worldPosText.setPosition(worldTxtPos);
		debugText1.setPosition(debugPos1);

		window.draw(screenPosText);
		window.draw(mapPosText);
		window.draw(worldPosText);
		window.draw(debugText1);

		if (showDebug) ml.Draw(window, tmx::MapLayer::Debug);//draw with debug info

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return -1;
}