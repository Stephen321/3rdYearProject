#include "Screens\OptionsScreen.h"

OptionsScreen::OptionsScreen(void)
{
}

void OptionsScreen::setTextOriginAndPosition(sf::Text &text, int multiplier, sf::Vector2f screenDimensions){
	//center text
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, 0);
	text.setPosition(sf::Vector2f(screenDimensions.x / 2.0f, (screenDimensions.y / 8.0f) * multiplier));
}

int OptionsScreen::Run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;

	window.setView(window.getDefaultView());

	sf::Font Font;
	sf::Text Menu1;
	int menu = 0;

	Font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	sf::Vector2f screenDimensions(window.getView().getSize());

	Menu1.setFont(Font);
	Menu1.setCharacterSize(70);
	Menu1.setString("Back");
	Menu1.setColor(sf::Color(255, 255, 255, 255));
	setTextOriginAndPosition(Menu1, 6, screenDimensions);
	int joystick = -1;

	while (Running)
	{
		if (sf::Joystick::isConnected(joystick) == false){
			for (int i = 0; i < 6; i++){
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
			if (sf::Joystick::isButtonPressed(joystick, 0)){
				if (menu == 0)
				{
					return (0);
				}
			}
			//Key pressed
			if (Event.type == sf::Event::KeyPressed)
			{
				switch (Event.key.code)
				{
				case sf::Keyboard::Up:
					if (menu > 0)
						//if (menu == 2 && menu == 2 /* <-- condition always true for test purposes, acual condition = no file to load */)
						//	menu -= 2;
						//else
						//	menu--;
					break;
				case sf::Keyboard::Down:
					if (menu < 3)
					{
						//if (menu == 0 && menu == 0 /* <-- condition always true for test purposes, acual condition = no file to load */)
						//	menu += 2;
						//else
						//	menu++;
					}
					break;
				default:
					break;
				}
			}
		}


		//Clearing screen
		window.clear();
		//Drawing

		window.draw(Menu1);

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}