#include "Screens\GameScreen.h"

GameScreen::GameScreen(void)
{
}

int GameScreen::Run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;
	bool showDebug = true;

	tmx::MapLoader ml("resources");
	ml.Load("test.tmx");
	ml.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

	sf::View view;
	view.reset(sf::FloatRect(0, 0, 443, 333));
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	window.setView(view);
	sf::Vector2f followPosition = view.getCenter();
	sf::Clock shaderClock, frameClock, deltaClock, box2dClock;
	b2World world(tmx::SfToBoxVec(sf::Vector2f(0.f, 0.f)));

	Player player(ml.IsometricToOrthogonal(sf::Vector2f(240, 400)), world);

	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

#pragma region Debug
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
	std::vector<std::unique_ptr<sf::Shape>> debugBoxes;
	std::vector<DebugShape> debugShapes;
#pragma endregion

	const std::vector<tmx::MapLayer>& layers = ml.GetLayers();
	for (const auto& l : layers)
	{
		if (l.name == "Collision")
		{
			for (const auto& o : l.objects)
			{
				b2Body* b = tmx::BodyCreator::Add(o, world);
				debugBoxes.push_back(std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(6.f, 6.f))));
				sf::Vector2f pos = tmx::BoxToSfVec(b->GetPosition());
				debugBoxes.back()->setPosition(pos);
				debugBoxes.back()->setOrigin(3.f, 3.f);

				for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
				{
					DebugShape ds;
					ds.setPosition(pos);
					b2PolygonShape* ps = (b2PolygonShape*)f->GetShape();
					int count = ps->GetVertexCount();
					for (int i = 0; i < count; i++)
						ds.AddVertex(sf::Vertex(tmx::BoxToSfVec(ps->GetVertex(i)), sf::Color::Red));
					if (count >=0) ds.AddVertex(sf::Vertex(tmx::BoxToSfVec(ps->GetVertex(0)), sf::Color::Red));
					debugShapes.push_back(ds);
				}
			}
		}
	}

	while (Running)
	{
#pragma region Debug
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
#pragma endregion
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
				default:
					break;
				}
			}
		}		

		//update stuff
		player.update(frameClock.restart());
		world.Step(box2dClock.restart().asSeconds(), 6, 3);

		//Clearing screen
		window.clear(sf::Color(0, 0, 0, 0));
		//Drawing
	
		view.setCenter(player.getPosition());
		window.setView(view);

		window.draw(ml);
		window.draw(player);
		
#pragma region Debug
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

		window.setView(view);
		if (showDebug) ml.Draw(window, tmx::MapLayer::Debug);//draw with debug info
		for (const auto& s : debugBoxes)
			window.draw(*s);
		for (const auto& s : debugShapes)
			window.draw(s);
#pragma endregion
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return -1;
}