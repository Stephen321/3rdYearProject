#include "Screens\GameScreen.h"

GameScreen::GameScreen(void)
{
}

int GameScreen::Run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;
	float zoom = 1.f;
	bool zoomed = false;

	tmx::MapLoader ml("resources");
	ml.Load("demo.tmx");
	ml.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

	sf::FloatRect viewRect = sf::FloatRect(0, 0, 800, 600);
	sf::View view;
	view.reset(viewRect);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	window.setView(view);
	window.setFramerateLimit(60);
	sf::Vector2f followPosition = view.getCenter();
	sf::Clock shaderClock, frameClock, deltaClock, box2dClock;
	MyListener contactListener;
	b2World world(tmx::SfToBoxVec(sf::Vector2f(0.f, 0.f)));
	world.SetContactListener(&contactListener);

	

	std::shared_ptr<GameData> ptr = GameData::getInstance();
	Player player(ml.IsometricToOrthogonal(sf::Vector2f(240, 400)), world);

	std::vector<std::unique_ptr<Character>> enemies;
	const int AICOUNT = 2;

	sf::Vector2f AB(ml.IsometricToOrthogonal(sf::Vector2f(ml.GetMapSize().x / 2.f, 0)));
	sf::Vector2f AD(ml.IsometricToOrthogonal(sf::Vector2f(0, ml.GetMapSize().y)));

	for (int i = 0; i < AICOUNT; i++)
	{
		float u = (std::rand() % 1001) / 1000.f;
		float b = (std::rand() % 1001) / 1000.f;
		sf::Vector2f pos = (u * AB) + (b * AD);
		enemies.push_back(std::make_unique<AI>(pos, world, &player));
	}

	std::vector<std::unique_ptr<Object>> objects;
	const int ROCKCOUNT = 20;

	AB -= ml.IsometricToOrthogonal(sf::Vector2f(ptr->rockTexture.getSize().x / 2.f, 0));
	AD -= ml.IsometricToOrthogonal(sf::Vector2f(0, ptr->rockTexture.getSize().y / 2.f));
	for (int i = 0; i < ROCKCOUNT; i++)
	{
		float u = (std::rand() % 1001) / 1000.f;
		float b = (std::rand() % 1001) / 1000.f;
		sf::Vector2f pos = (u * AB) + (b * AD);
		objects.push_back(std::make_unique<Rock>(world, pos));
	}

	window.setKeyRepeatEnabled(false);
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
	std::vector<std::unique_ptr<sf::Shape>> debugBoxes;
	std::vector<DebugShape> debugShapes;

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


	int joystick = -1;
	while (Running)
	{
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

		if (sf::Joystick::isConnected(joystick) == false){
			for (int i = 0; i < 6; i++){
				if (sf::Joystick::isConnected(i)){
					joystick = i;
					sf::Joystick::Identification id = sf::Joystick::getIdentification(joystick);
					std::cout << (id.name).toAnsiString() << std::endl;
					std::cout << id.productId << std::endl;
					std::cout << id.vendorId << std::endl;
				}
			}
		}
		//Verifying events
		while (window.pollEvent(Event))
		{
			player.setEvent(Event);
			// Window closed
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}

			if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::D))
				Debug::displayInfo = !Debug::displayInfo;
			if (sf::Joystick::isButtonPressed(joystick, 7))
			{
				return 0;
			}
			if (Event.type == sf::Event::MouseWheelScrolled)
			{
				if (Event.mouseWheelScroll.delta < 0 && zoom > 1)
					zoom = 1;
				else if (Event.mouseWheelScroll.delta > 0 && zoom < 1)
					zoom = 1;
				zoom += Event.mouseWheelScroll.delta / 100.f;
				zoomed = true;
			}
			if (Event.type == sf::Event::MouseButtonReleased)
			{
				if (Event.mouseButton.button == sf::Mouse::Button::Right)
					view.reset(viewRect);
			}
		}		

		//update stuff
		sf::Time dt = frameClock.restart();

		sf::FloatRect bounds;
		bounds.left = view.getCenter().x - view.getSize().x / 2.f;
		bounds.top = view.getCenter().y - view.getSize().y / 2.f;
		bounds.width = view.getSize().x;
		bounds.height = view.getSize().y;

		player.update(dt, bounds);
		for (const std::unique_ptr<Character>& c : enemies)
			c->update(dt, bounds);
		for (const std::unique_ptr<Object>& o : objects)
			o->update(bounds);

		world.Step(box2dClock.restart().asSeconds(), 6, 3);

		//Clearing screen
		window.clear(sf::Color(0, 0, 0, 0));
		//Drawing
	
		view.setCenter((int)player.getPosition().x, (int)player.getPosition().y);
		if (zoomed){
			view.zoom(zoom);
			zoomed = false;
		}
		window.setView(view);

		window.draw(ml);
		/*window.draw(player);
		for (const std::unique_ptr<Character>& c : enemies)
			window.draw(*c);*/
		std::vector<VisibleObject*> visibleChars;
		visibleChars.push_back(&player);
		for (const std::unique_ptr<Character>& c : enemies){
			if (c->getVisible())
				visibleChars.push_back(c.get());
		}
		for (const std::unique_ptr<Object>& o : objects){
			if (o->getVisible())
				visibleChars.push_back(o.get());
		}
		std::sort(visibleChars.begin(), visibleChars.end(), [](const VisibleObject* v1, const VisibleObject* v2)->bool
		{
			return (v1->getPosition().y < v2->getPosition().y);
		});

		for (const VisibleObject* v : visibleChars)
			window.draw(*v);		

		if (Debug::displayInfo){
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
			ml.Draw(window, tmx::MapLayer::Debug);//draw with debug info
			for (const auto& s : debugBoxes)
				window.draw(*s);
			for (const auto& s : debugShapes)
				window.draw(s);
		}

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return -1;
}