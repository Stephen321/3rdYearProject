#include "CreditsScreen.h"

CreditsScreen::CreditsScreen(void)
{

}

int CreditsScreen::Run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;	

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
				case sf::Keyboard::Escape:
					return (0);
					break;				
				case sf::Keyboard::Return:
					return 0;
					break;
				default:
					break;
				}
			}
		}
		

		//Clearing screen
		window.clear();
		//Drawing		
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}