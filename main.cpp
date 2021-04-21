#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Controller.h"
using namespace sf;
using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1400, 764), "Project3");

	Controller runner;
	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			window.clear(Color(207, 255, 239, 255));

			runner.update(window);
			
			window.display();
		}


	}

	return 0;
}