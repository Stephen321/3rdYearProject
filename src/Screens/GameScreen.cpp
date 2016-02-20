#include "Screens\GameScreen.h"

GameScreen::GameScreen(MapLoader * ml, Pathfinder * pf) :
m_mapLoader(ml),
m_pathFinder(pf)
{
}

int GameScreen::Run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;
	float zoom = 1.f;
	bool zoomed = false;

	std::shared_ptr<SoundManager> sndMgr = SoundManager::getInstance();
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	//Pathfinder * pf = ml->getPathFinder();
	//sound 
	sf::Clock soundClock;
	const int SOUND_DELAY = 4;
	int soundPlayDelay = 3 + (rand() % SOUND_DELAY);
	sf::CircleShape birdCircle(20);
	birdCircle.setPosition(m_mapLoader->isometricToOrthogonal(sf::Vector2f(240, 350)));
	birdCircle.setFillColor(sf::Color::Cyan);
	birdCircle.setOrigin(20.f, 20.f);

	//view
	sf::FloatRect viewRect = sf::FloatRect(0, 0, 800, 600);
	sf::View view;
	view.reset(viewRect);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	window.setView(view);

	//clocks
	sf::Clock shaderClock, frameClock, deltaClock, box2dClock;

	//box2d
	MyListener contactListener;
	b2World world(SfToBoxVec(sf::Vector2f(0.f, 0.f)));
	world.SetContactListener(&contactListener);	


	//ml.UpdateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

	//characters
	Player player(world);
	std::vector<std::shared_ptr<Character>> enemies;

	//sound
	sndMgr->setListener(&player);

	//game objects
	std::vector<std::shared_ptr<GameObject>> gameObjects;

	window.setKeyRepeatEnabled(false);

	sf::Clock successTimer;

	//text
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
	sf::Text succesText;
	succesText.setFont(font);
	succesText.setStyle(sf::Text::Regular);
	succesText.setCharacterSize(80);
	succesText.setOrigin(succesText.getGlobalBounds().width / 2.f, succesText.getGlobalBounds().height / 2.f);
	succesText.setPosition(view.getCenter());
	succesText.setString("");
	succesText.setPosition(succesText.getPosition().x - 100, succesText.getPosition().y);

	//sound
	sndMgr->playSound("wind_ambience", true, 0.6f);
	
	//debug
	std::vector<std::unique_ptr<sf::Shape>> debugBoxes;
	std::vector<DebugShape> debugShapes;

	//loop through layers and objects
	const std::vector<MapLayer>& layers = m_mapLoader->getLayers();
	for (const auto& l : layers)
	{
		if (l.getName() == "Collision")
		{
			for (const auto& o : l.objects)
			{
				b2Body* b = BodyCreator::Add(o, world);
				debugBoxes.push_back(std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(6.f, 6.f))));
				sf::Vector2f pos = BoxToSfVec(b->GetPosition());
				debugBoxes.back()->setPosition(pos);
				debugBoxes.back()->setOrigin(3.f, 3.f);

				for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
				{
					DebugShape ds;
					ds.setPosition(pos);
					b2PolygonShape* ps = (b2PolygonShape*)f->GetShape();
					int count = ps->GetVertexCount();
					for (int i = 0; i < count; i++)
						ds.AddVertex(sf::Vertex(BoxToSfVec(ps->GetVertex(i)), sf::Color::Red));
					if (count >= 0) ds.AddVertex(sf::Vertex(BoxToSfVec(ps->GetVertex(0)), sf::Color::Red));
					debugShapes.push_back(ds);
				}
			}
		}
		if (l.getName() == "Entities")
		{
			for (const auto& o : l.objects)
			{
				if (o.GetName() == "Player")
					player.setPosition(o.GetCentre());
				else if (o.GetName() == "Ai"){
					enemies.push_back(std::make_shared<AI>(world, &player, o.GetCentre(), m_pathFinder));
				}
			}
		}
		if (l.getName() == "GameObjects")
		{
			for (const auto& o : l.objects)
			{
				if (o.GetName() == "Rock")
				{
					gameObjects.push_back(std::make_shared<Rock>(world, o.GetPosition()));
				}
			}
		}
	}


	int joystick = -1;

	while (Running)
	{
		sndMgr->update();
		window.setView(view); //need to change view back to mouse pos info is correct
		sf::Vector2f mouseScreenPos = (sf::Vector2f)sf::Mouse::getPosition(window);
		screenPosText.setString("ScreenPos: (" + std::to_string((int)mouseScreenPos.x) + ", " +
		std::to_string((int)mouseScreenPos.y) + ")");
		sf::Vector2f mouseMapPos = m_mapLoader->orthogonalToIsometric(window.mapPixelToCoords((sf::Vector2i)mouseScreenPos));
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
			player.handleEvent(Event);

			// Window closed
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}
			if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::D))
				Debug::displayInfo = !Debug::displayInfo;

			if ((Event.type == sf::Event::KeyReleased) && (Event.key.code == sf::Keyboard::P))
				Debug::displayPathfinder = !Debug::displayPathfinder;
			if (Event.type == sf::Event::EventType::JoystickButtonPressed && Event.joystickButton.button == 7)
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
		world.Step(1.f / 60.f, 8, 3);
		//world.Step(box2dClock.restart().asSeconds(), 6, 3);

		//update sound
		if (soundClock.getElapsedTime().asSeconds() > soundPlayDelay){
			soundPlayDelay = 1 + (rand() % SOUND_DELAY);
			soundClock.restart();
			int soundDistance = 50;
			int minDistace = 35;
			sf::Vector2f soundOffset((rand() % ((2 * soundDistance) + 1)) - soundDistance + minDistace,
									 (rand() % ((2 * soundDistance) + 1)) - soundDistance + minDistace);
			birdCircle.setPosition(player.getPosition() + soundOffset);
			int soundToPlay = 1 + (rand() % 5);
			sndMgr->playSound("bird_tweet_" + std::to_string(soundToPlay), false, 0.5f, birdCircle.getPosition());
		}

		//update stuff
		sf::Time dt = frameClock.restart();

		sf::FloatRect bounds;
		bounds.left = view.getCenter().x - view.getSize().x / 2.f;
		bounds.top = view.getCenter().y - view.getSize().y / 2.f;
		bounds.width = view.getSize().x;
		bounds.height = view.getSize().y;

		player.update(dt, bounds);  
		for (const std::shared_ptr<Character>& c : enemies)
			if (c->getAlive())
			c->update(dt, bounds);
		for (const std::shared_ptr<GameObject>& o : gameObjects)
			o->update(bounds);


		if (player.getAlive() == false && succesText.getString() == ""){
			succesText.setString("You lost!");
			successTimer.restart();
		}


		bool enemiesDead = true;
		for (auto itr = enemies.begin(); itr != enemies.end(); ++itr){
			if ((*itr)->getAlive()){
				enemiesDead = false;
				break;
			}
		}

		if (enemiesDead && succesText.getString() == ""){//player won
			succesText.setString("You won!");
			successTimer.restart();
		}

		if (succesText.getString() != "" && successTimer.getElapsedTime().asSeconds() > 4.5f){
			succesText.setString("");
			const std::vector<MapLayer>& layers = m_mapLoader->getLayers();
			for (const auto& l : layers)
			{
				if (l.getName() == "Entities")
				{
					int i = 0;
					for (const auto& o : l.objects)
					{
						if (o.GetName() == "Player")
							player.reset(o.GetPosition());
						else
							enemies[i++]->reset(o.GetPosition());
					}
					break;
				}
			}
		}


		//Clearing screen
		window.clear(sf::Color(0, 0, 0, 0));
		//Drawing
	
		view.setCenter(player.getPosition());
		if (zoomed){
			view.zoom(zoom);
			zoomed = false;
		}


		window.setView(view);
		m_mapLoader->Draw(window);
		if (Debug::displayPathfinder)
			m_pathFinder->drawNodes(window);

		std::vector<VisibleObject*> visibleChars;
		visibleChars.push_back(&player);
		for (const std::shared_ptr<Character>& c : enemies){
			if (c->getAlive() && c->getVisible())
				visibleChars.push_back(c.get());
		}
		for (const std::shared_ptr<GameObject>& o : gameObjects){
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
			sf::Vector2f mapTxtPos = window.getView().getCenter() + sf::Vector2f(((window.getView().getSize().x / 2) - 175), (-window.getView().getSize().y / 2) + 25);
			sf::Vector2f worldTxtPos = window.getView().getCenter() + sf::Vector2f(((window.getView().getSize().x / 2) - 195), (-window.getView().getSize().y / 2) + 50);
			sf::Vector2f debugPos1 = window.getView().getCenter() + sf::Vector2f(((window.getView().getSize().x / 2) - 195), (-window.getView().getSize().y / 2) + 75);
			screenPosText.setPosition(screenTxtPos);
			mapPosText.setPosition(mapTxtPos);
			worldPosText.setPosition(worldTxtPos);
			debugText1.setPosition(debugPos1);

			window.draw(screenPosText);
			window.draw(mapPosText);
			window.draw(worldPosText);
			window.draw(debugText1);

			window.setView(view);
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