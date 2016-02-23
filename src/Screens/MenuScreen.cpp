#include "Screens\MenuScreen.h"
#include <iostream> //testing
MenuScreen::MenuScreen(void)
{

	std::shared_ptr<GameData> ptr = GameData::getInstance();

	playButton = sf::Sprite(ptr->menuPlay);
	optionsButton = sf::Sprite(ptr->menuOptions);
	quitButton = sf::Sprite(ptr->menuQuit);
	playSelected = sf::Sprite(ptr->playSelected);
	optionsSelected = sf::Sprite(ptr->optionsSelected);
	quitSelected = sf::Sprite(ptr->quitSelected);
	centreSpriteOrigin(playButton);
	centreSpriteOrigin(optionsButton);
	centreSpriteOrigin(quitButton);
	centreSpriteOrigin(playSelected);
	centreSpriteOrigin(optionsSelected);
	centreSpriteOrigin(quitSelected);
	playButton.setScale(0, 0);
	optionsButton.setScale(0, 0);
	quitButton.setScale(0, 0);
	playSelected.setColor(sf::Color(255, 255, 255, 0));
	optionsSelected.setColor(sf::Color(255, 255, 255, 0));
	playSelected.setColor(sf::Color(255, 255, 255, 0));
	quitSelected.setColor(sf::Color(255, 255, 255, 0));

	blackGradient = sf::Sprite(ptr->menuBlackGradient);
	blackGradient.setOrigin(0, blackGradient.getTextureRect().height);

	unfolded = sf::Sprite(ptr->menuUnfolded);
	centreSpriteOrigin(unfolded);

	titleLetters = sf::Sprite(ptr->menuLetters);
	std::string title = "Malevolence";
	for (int i = 0; i < title.length(); i++)
	{
		sf::Sprite s;
		s.setTexture(ptr->menuLetters);
		s.setTextureRect(sf::IntRect(i * titleLetters.getTextureRect().width / title.length(), 0,
			titleLetters.getTextureRect().width / title.length(), titleLetters.getTextureRect().height));
		s.setOrigin(sf::Vector2f(s.getTextureRect().width / 2, s.getTextureRect().height / 2));

		springs.push_back(SpringObject(s));
	}

	springs.push_back(SpringObject(unfolded));

	springs[0].setRestingPos(-300, -330);
	springs[1].setRestingPos(-220, -355);
	springs[2].setRestingPos(-165, -360);
	springs[3].setRestingPos(-110, -365);
	springs[4].setRestingPos(-55, -370);
	springs[5].setRestingPos(+20, -370);
	springs[6].setRestingPos(+80, -370);
	springs[7].setRestingPos(+130, -365);
	springs[8].setRestingPos(+195, -360);
	springs[9].setRestingPos(+265, -355);
	springs[10].setRestingPos(+325, -340);
	springs[11].setRestingPos(-10, -240);
}

void MenuScreen::centreSpriteOrigin(sf::Sprite & sprite)
{
	sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
}

void MenuScreen::setTextOriginAndPosition(sf::Text &text, int multiplier, sf::Vector2f screenDimensions){
	//center text
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, 0);
	text.setPosition(sf::Vector2f(screenDimensions.x / 2.0f, (screenDimensions.y / 8.0f) * multiplier));
}

int MenuScreen::Run(sf::RenderWindow &window)
{
	blackGradient.setPosition(sf::Vector2f(0, window.getSize().y));
	unfolded.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f - 100)); // +125));

	int buttonOffsetY = 10;
	playButton.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f + buttonOffsetY));
	optionsButton.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f + 105 + buttonOffsetY));
	quitButton.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f + 210 + buttonOffsetY));

	playSelected.setPosition(playButton.getPosition());
	optionsSelected.setPosition(optionsButton.getPosition());
	quitSelected.setPosition(quitButton.getPosition());

	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	float timer = 0;
	float menuItemsTimer = 0;
	int counter = 0;

	window.setView(window.getDefaultView());

	sf::Font Font;
	sf::Text Menu1, Menu2, Menu3, Menu4;
	int menu = 0;

	Font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	sf::Vector2f screenDimensions(window.getView().getSize());

	Menu1.setFont(Font);
	Menu1.setCharacterSize(70);
	Menu1.setString("Play");
	Menu1.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu1, 3, screenDimensions);

	Menu2.setFont(Font);
	Menu2.setCharacterSize(70);
	Menu2.setString("Continue");
	Menu2.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu2, 4, screenDimensions);

	Menu3.setFont(Font);
	Menu3.setCharacterSize(70);
	Menu3.setString("Options");
	Menu3.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu3, 5, screenDimensions);

	Menu4.setFont(Font);
	Menu4.setCharacterSize(70);
	Menu4.setString("Exit");
	Menu4.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu4, 6, screenDimensions);
	bool joyStickAlreadyMoved = false;
	int joystick = -1;

	while (Running)
	{
		float dt = frameClock.restart().asSeconds();
		for (int i = 0; i < springs.size(); i++)
		{
			springs[i].update(dt);
		}

		if (timer != -1)
			timer += dt;
		else
			menuItemsTimer += dt;

		if (counter < springs.size() - 1)
		{

			if (timer > 0.5f)
			{
				springs[counter].setReleased();
				counter++;
				timer = 0;
			}
		}

		if (timer > 3.0f)
		{
			springs[springs.size() - 1].setReleased();
			timer = -1;
		}

		if (menuItemsTimer > 2.0f && setPlayScale == false)
			setPlayScale = true;
		else if (menuItemsTimer > 2.75f && setOptionsScale == false)
			setOptionsScale = true;
		else if (menuItemsTimer > 3.5f && setQuitScale == false)
			setQuitScale = true;
		else if (menuItemsTimer > 4.0f && menuLoaded == false)
			menuLoaded = true;

		if (setPlayScale && playButton.getScale().x < 1)
			playButton.setScale(playButton.getScale().x + 0.05f, playButton.getScale().y + 0.05f);
		if (setOptionsScale && optionsButton.getScale().x < 1)
			optionsButton.setScale(optionsButton.getScale().x + 0.05f, optionsButton.getScale().y + 0.05f);
		if (setQuitScale && quitButton.getScale().x < 1)
			quitButton.setScale(quitButton.getScale().x + 0.05f, quitButton.getScale().y + 0.05f);

		if (sf::Joystick::isConnected(joystick) == false){
			for (int i = 0; i < 6 && joystick == -1; i++){
				if (sf::Joystick::isConnected(i))
					joystick = i;
			}
		}
		//Verifying events
		while (window.pollEvent(Event))
		{
			// Window closed
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}
			//Key pressed
			float yPos = 0;
			if (Event.type == sf::Event::JoystickMoved && Event.joystickMove.axis == sf::Joystick::Y)
			{
				yPos = Event.joystickMove.position;
			}
			bool joystickMoved = yPos > 10 || yPos < -10;
			if (!joystickMoved)
				joyStickAlreadyMoved = false;

			if (joystickMoved)
			{
				if (yPos < -10 && joyStickAlreadyMoved == false){ //up
					if (menu > 0){
						menu--;
						joyStickAlreadyMoved = true;
					}
				}
				else if (yPos > 10 && joyStickAlreadyMoved == false){ //down
					if (menu < 2)
					{
						menu++;
						joyStickAlreadyMoved = true;
					}
				}
			}
			if ((Event.type == sf::Event::JoystickButtonPressed && Event.joystickButton.button == 0 && menuLoaded)){
				if (menu == 0)
				{
					return (1);
				}
				else if (menu == 1)
				{
					//load from file
					return (2);
				}
				else
				{
					return (-1);
				}
			}
		}

		//Clearing screen
		window.clear();
		//Drawing

		for (int i = 0; i < springs.size(); i++)
		{
			window.draw(springs[i]);
		}

		window.draw(blackGradient);

		window.draw(playButton);
		window.draw(optionsButton);
		window.draw(quitButton);

		if (menuLoaded)
		{
			if (menu == 0)
			{
				if (playSelected.getColor().a < 255)
					playSelected.setColor(sf::Color(255, 255, 255, playSelected.getColor().a + 5));
				if (optionsSelected.getColor().a > 0)
					optionsSelected.setColor(sf::Color(255, 255, 255, optionsSelected.getColor().a - 5));
				if (quitSelected.getColor().a > 0)
					quitSelected.setColor(sf::Color(255, 255, 255, quitSelected.getColor().a - 5));
			}
			else if (menu == 1)
			{
				if (playSelected.getColor().a > 0)
					playSelected.setColor(sf::Color(255, 255, 255, playSelected.getColor().a - 5));
				if (optionsSelected.getColor().a < 255)
					optionsSelected.setColor(sf::Color(255, 255, 255, optionsSelected.getColor().a + 5));
				if (quitSelected.getColor().a > 0)
					quitSelected.setColor(sf::Color(255, 255, 255, quitSelected.getColor().a - 5));
			}
			else if (menu == 2)
			{
				if (playSelected.getColor().a > 0)
					playSelected.setColor(sf::Color(255, 255, 255, playSelected.getColor().a - 5));
				if (optionsSelected.getColor().a > 0)
					optionsSelected.setColor(sf::Color(255, 255, 255, optionsSelected.getColor().a - 5));
				if (quitSelected.getColor().a < 255)
					quitSelected.setColor(sf::Color(255, 255, 255, quitSelected.getColor().a + 5));
			}

			window.draw(playSelected);
			window.draw(optionsSelected);
			window.draw(quitSelected);
		}

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}