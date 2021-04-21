#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;

class Button
{
public:
	Sprite background;
	Sprite foreground;
	
	
	bool ischosen;

	Button(map<string, Texture> &textures, float xpos, float ypos);
		
	void toggleChosen();
	
	void draw(RenderWindow &w);
};

Button::Button(map<string, Texture> &textures, float xpos, float ypos)
{
	background.setPosition(xpos, ypos);
	foreground.setPosition(xpos, ypos);

	foreground.setTexture(textures["selected"]);
	background.setTexture(textures["unselected"]);
	foreground.setColor(Color(255, 255, 255, 0));
	ischosen = false;
}

void Button::toggleChosen()
{
	
		if (ischosen)
		{
			ischosen = false;
			foreground.setColor(Color(255, 255, 255, 0));
		}
		else
		{
			ischosen = true;
			foreground.setColor(Color(255, 255, 255, 255));
		}
	
}

void Button::draw(RenderWindow &w)
{
	w.draw(background);
	w.draw(foreground);
	
	
}