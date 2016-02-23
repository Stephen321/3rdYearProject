#include "Screens\MenuScreen.h"
#include <iostream> //testing
MenuScreen::MenuScreen(void)
{
	std::shared_ptr<GameData> ptr = GameData::getInstance(); //pointer to gameData so we can initialse the sprites

	//initialise sprites with textures from gameData
	playButton = sf::Sprite(ptr->menuPlay);
	optionsButton = sf::Sprite(ptr->menuOptions);
	quitButton = sf::Sprite(ptr->menuQuit);
	playSelected = sf::Sprite(ptr->playSelected);
	optionsSelected = sf::Sprite(ptr->optionsSelected);
	quitSelected = sf::Sprite(ptr->quitSelected);

	//make the origin of these sprites centred
	centreSpriteOrigin(playButton);
	centreSpriteOrigin(optionsButton);
	centreSpriteOrigin(quitButton);
	centreSpriteOrigin(playSelected);
	centreSpriteOrigin(optionsSelected);
	centreSpriteOrigin(quitSelected);

	//initialse menu options to be scaled to 0 for their transition in
	playButton.setScale(0, 0);
	optionsButton.setScale(0, 0);
	quitButton.setScale(0, 0);

	//make the glow effect for menu options selection invisible to begin with
	playSelected.setColor(sf::Color(255, 255, 255, 0));
	optionsSelected.setColor(sf::Color(255, 255, 255, 0));
	quitSelected.setColor(sf::Color(255, 255, 255, 0));

	blackGradient = sf::Sprite(ptr->menuBlackGradient);
	blackGradient.setOrigin(0, blackGradient.getTextureRect().height);

	unfolded = sf::Sprite(ptr->menuUnfolded);
	centreSpriteOrigin(unfolded);

	//create a collection of springs by inserting an entry for each letter in "Malevolence" into springs
	titleLetters = sf::Sprite(ptr->menuLetters);
	std::string title = "Malevolence";
	for (int i = 0; i < title.length(); i++)
	{
		//create and pass in a sprite to each spring
		sf::Sprite s;
		s.setTexture(ptr->menuLetters);
		s.setTextureRect(sf::IntRect(i * titleLetters.getTextureRect().width / title.length(), 0,
			titleLetters.getTextureRect().width / title.length(), titleLetters.getTextureRect().height));
		s.setOrigin(sf::Vector2f(s.getTextureRect().width / 2, s.getTextureRect().height / 2));

		springs.push_back(SpringObject(s));
	}

	springs.push_back(SpringObject(unfolded)); //add one more spring for the "Unfolded" sprite

	//manually set the resting position for each spring so they rest at different positions
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

//positions a sprites origin in the centre of the texture
void MenuScreen::centreSpriteOrigin(sf::Sprite & sprite)
{
	sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
}

int MenuScreen::Run(sf::RenderWindow &window)
{
	blackGradient.setPosition(sf::Vector2f(0, window.getSize().y));
	unfolded.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f - 100));

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
	
	int menu = 0;


	bool joyStickAlreadyMoved = false;
	int joystick = -1;

	while (Running)
	{
		float dt = frameClock.restart().asSeconds();
		for (int i = 0; i < springs.size(); i++)
		{
			springs[i].update(dt);
		}


		if (menuLoaded == false) //we only want to do this once
		{
			//using 2 timers as there's 2 distinct phases of this sequence we need them for
			if (timer != -1)
				timer += dt;
			else
				menuItemsTimer += dt;

			if (counter < springs.size() - 1) //sequentially release each letter in "Malevolence" so their springs come to rest one by one
			{
				if (timer > 0.5f) //half a second delay between 
				{
					springs[counter].setReleased();
					counter++;
					timer = 0;
				}
			}

			if (timer > 3.0f) //transition in the text "Unfolded" after a short period
			{
				springs[springs.size() - 1].setReleased();
				timer = -1; //no need to increment this timer any more
			}

			//use a timer so the menu options scale up in a sequence
			if (menuItemsTimer > 2.0f && setPlayScale == false)
				setPlayScale = true;
			else if (menuItemsTimer > 2.75f && setOptionsScale == false)
				setOptionsScale = true;
			else if (menuItemsTimer > 3.5f && setQuitScale == false)
				setQuitScale = true;
			else if (menuItemsTimer > 4.0f && menuLoaded == false)
				menuLoaded = true; //we don't want the menu code reactivating when we're this far in

			//transition the menu options onto the screen by increasing their scale from 0 to 1
			if (setPlayScale && playButton.getScale().x < 1)
				playButton.setScale(playButton.getScale().x + 0.05f, playButton.getScale().y + 0.05f);
			if (setOptionsScale && optionsButton.getScale().x < 1)
				optionsButton.setScale(optionsButton.getScale().x + 0.05f, optionsButton.getScale().y + 0.05f);
			if (setQuitScale && quitButton.getScale().x < 1)
				quitButton.setScale(quitButton.getScale().x + 0.05f, quitButton.getScale().y + 0.05f);

		}

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
			//Depending which option is selcted, fade in or out the glowing selected effects on each menu item
			if (menu == 0)
			{
				if (playSelected.getColor().a < 255)
					playSelected.setColor(sf::Color(255, 255, 255, playSelected.getColor().a + 5)); //fade in
				if (optionsSelected.getColor().a > 0)
					optionsSelected.setColor(sf::Color(255, 255, 255, optionsSelected.getColor().a - 5)); //fade out
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

			//draw the glowing effects
			window.draw(playSelected);
			window.draw(optionsSelected);
			window.draw(quitSelected);
		}

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}