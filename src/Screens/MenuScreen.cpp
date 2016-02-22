#include "Screens\MenuScreen.h"
#include <iostream> //testing
MenuScreen::MenuScreen(void)
{		

	std::shared_ptr<GameData> ptr = GameData::getInstance();

	playButton = sf::Sprite(ptr->menuPlay);
	optionsButton = sf::Sprite(ptr->menuOptions);
	quitButton = sf::Sprite(ptr->menuQuit);
	centreSpriteOrigin(playButton);
	centreSpriteOrigin(optionsButton);
	centreSpriteOrigin(quitButton);

	blackGradient = sf::Sprite(ptr->menuBlackGradient);
	blackGradient.setOrigin(0, blackGradient.getTextureRect().height);

	unfolded = sf::Sprite(ptr->menuUnfolded);
	centreSpriteOrigin(unfolded);

	letterM = sf::Sprite(ptr->menuLetterM);
	letterA = sf::Sprite(ptr->menuLetterA);
	letterL1 = sf::Sprite(ptr->menuLetterL1);
	letterE1 = sf::Sprite(ptr->menuLetterE1);
	letterV = sf::Sprite(ptr->menuLetterV);
	letterO = sf::Sprite(ptr->menuLetterO);
	letterL2 = sf::Sprite(ptr->menuLetterL2);
	letterE2 = sf::Sprite(ptr->menuLetterE2);
	letterN = sf::Sprite(ptr->menuLetterN);
	letterC = sf::Sprite(ptr->menuLetterC);
	letterE3 = sf::Sprite(ptr->menuLetterE3);
	centreSpriteOrigin(letterM);
	centreSpriteOrigin(letterA);
	centreSpriteOrigin(letterL1);
	centreSpriteOrigin(letterE1);
	centreSpriteOrigin(letterV);
	centreSpriteOrigin(letterO);
	centreSpriteOrigin(letterL2);
	centreSpriteOrigin(letterE2);
	centreSpriteOrigin(letterN);
	centreSpriteOrigin(letterC);
	centreSpriteOrigin(letterE3);

	m_origin = sf::Vector2f(0, 0);
	m_natPosition = sf::Vector2f(m_origin.x + 100, m_origin.y + 100);

	m_position = sf::Vector2f(m_origin.x + 100, m_origin.y + 300);
	m_velocity = sf::Vector2f(0, 0);
	accel = sf::Vector2f(0, 0);
	m_dampingConst = 0.4f;
	m_springConst = 0.05f;
	m_mass = 100;
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

	sf::Event Event;
	bool Running = true;	

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
		float positionLength = sqrt(m_position.x * m_position.x + m_position.y * m_position.y);
		float natLength = sqrt(m_natPosition.x * m_natPosition.x + m_natPosition.y * m_natPosition.y);

		accel = -(m_springConst / m_mass)*(positionLength - natLength)* m_position / positionLength - (m_dampingConst / m_mass)* m_velocity;
		m_velocity += accel;
		m_position += m_velocity;
		std::cout << accel.y << std::endl;

		letterM.setPosition(m_position);


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
						if (menu == 2 && menu == 2 /* <-- condition always true for test purposes, acual condition = no file to load */)
							menu -= 2;
						else
							menu--;
						joyStickAlreadyMoved = true;
					}
				}
				else if (yPos > 10 && joyStickAlreadyMoved == false){ //down
					if (menu < 3)
					{
						if (menu == 0 && menu == 0 /* <-- condition always true for test purposes, acual condition = no file to load */)
							menu += 2;
						else
							menu++;
						joyStickAlreadyMoved = true;
					}
				}
			}
			if ((Event.type == sf::Event::JoystickButtonPressed && Event.joystickButton.button == 0)){
				if (menu == 0)
				{
					return (1);
				}
				else if (menu == 1)
				{
					//load from file
					return (1);
				}
				else if (menu == 2)
				{
					return (2);
				}
				else
					return (-1);
			}

		}		
		
		if (menu == 0)
		{
			Menu1.setColor(sf::Color(255, 255, 255, 255));
			Menu2.setColor(sf::Color(100, 100, 100, 255));
			Menu3.setColor(sf::Color(100, 100, 100, 255));
			Menu4.setColor(sf::Color(100, 100, 100, 255));
		}
		else if (menu == 1)
		{
			Menu1.setColor(sf::Color(100, 100, 100, 255));
			Menu2.setColor(sf::Color(255, 255, 255, 255));
			Menu3.setColor(sf::Color(100, 100, 100, 255));
			Menu4.setColor(sf::Color(100, 100, 100, 255));
		}
		else if (menu == 2)
		{
			Menu1.setColor(sf::Color(100, 100, 100, 255));
			Menu2.setColor(sf::Color(100, 100, 100, 255));
			Menu3.setColor(sf::Color(255, 255, 255, 255));
			Menu4.setColor(sf::Color(100, 100, 100, 255));
		}
		else
		{
			Menu1.setColor(sf::Color(100, 100, 100, 255));
			Menu2.setColor(sf::Color(100, 100, 100, 255));
			Menu3.setColor(sf::Color(100, 100, 100, 255));
			Menu4.setColor(sf::Color(255, 255, 255, 255));
		}
		

		//Clearing screen
		window.clear();
		//Drawing
			
		window.draw(Menu1);			
			
		window.draw(Menu2);
			
		window.draw(Menu3);

		window.draw(Menu4);

		window.draw(letterM);
		window.draw(letterA);
		window.draw(letterL1);
		window.draw(letterE1);
		window.draw(letterV);
		window.draw(letterO);
		window.draw(letterL2);
		window.draw(letterE2);
		window.draw(letterN);
		window.draw(letterC);
		window.draw(letterE3);
		window.draw(unfolded);

		window.draw(blackGradient);

		window.draw(playButton);
		window.draw(optionsButton);
		window.draw(quitButton);
		
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}