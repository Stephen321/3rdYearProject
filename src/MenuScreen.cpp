#include "MenuScreen.h"

MenuScreen::MenuScreen(void)
{		
}

void MenuScreen::setTextOriginAndPosition(sf::Text &text, int multiplier, float screenWidth, float screenHeight){
	//center text
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, 0);
	text.setPosition(sf::Vector2f(screenWidth / 2.0f, (screenHeight / 8.0f) * multiplier));
}

int MenuScreen::Run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;

	screenWidth = window.getView().getSize().x;
	screenHeight = window.getView().getSize().y;

	int alpha = 0;
	sf::Font Font;
	sf::Text Menu1;
	sf::Text Menu2;
	sf::Text Menu3;
	sf::Text Menu4;
	int menu = 0;

	Font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	Menu1.setFont(Font);
	Menu1.setCharacterSize(70);
	Menu1.setString("Play");
	Menu1.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu1, 3, screenWidth, screenHeight);

	Menu2.setFont(Font);
	Menu2.setCharacterSize(70);
	Menu2.setString("Continue");
	Menu2.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu2, 4, screenWidth, screenHeight);

	Menu3.setFont(Font);
	Menu3.setCharacterSize(70);
	Menu3.setString("Options");
	Menu3.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu3, 5, screenWidth, screenHeight);

	Menu4.setFont(Font);
	Menu4.setCharacterSize(70);
	Menu4.setString("Exit");
	Menu4.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu4, 6, screenWidth, screenHeight);

	while (Running)
	{
		//Verifying events
		while (window.pollEvent(Event))
		{
			// Window closed
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}
			//Key pressed
			if (Event.type == sf::Event::KeyPressed)
			{
				switch (Event.key.code)
				{				
				case sf::Keyboard::Up:
					if (menu > 0)
						if (menu == 2 && menu == 2 /* <-- condition always true for test purposes, acual condition = no file to load */)
							menu -= 2;
						else
							menu--;
					break;
				case sf::Keyboard::Down:
					if (menu < 3)
					{
						if (menu == 0 && menu == 0 /* <-- condition always true for test purposes, acual condition = no file to load */)
							menu += 2;
						else
							menu++;
					}
					break;
				case sf::Keyboard::Return:
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
					break;
				default:
					break;
				}
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
		
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}