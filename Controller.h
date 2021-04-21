#pragma once
#include "Button.h"
#include "Graph.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>


using namespace std;
using namespace sf;

//front end-Harrison
class Controller
{
	map<string, Texture> icontextures;

	//all of these are sprites used to give the options
	Sprite rectangleword;
	Sprite triangleword;
	Sprite pentagonword;
	Sprite hexagonword;
	Sprite octagonword;
	Sprite wordinst;
	Sprite algo1;
	Sprite algo2;
	vector<Sprite> shapewords;
	vector<Sprite> algowords;
	vector<Sprite> nodeselect;
	Sprite word100;
	Sprite word10000;
	Sprite word100000;
	Sprite nodeinst;

	Graph *g100 = new Graph(100); //initialize a graph upon start, 100 nodes
	Hash *hash100 = new Hash(361, g100);
	
public:
	string algo1time, algo2time;
	double time_elapsed1, time_elapsed2;
	bool found = false; //if solutions have been found
	int numofNodes;

	vector<Button> buttons; //buttons for chosing which shape
	vector<Button> nodebutts; //button for chosing number of nodes
	

	vector<vector<Node>> highlighted; //This is the output from the algorithms Tristan made, will return all the nodes which make up the desired shape and will be highlighted
	vector<vector<int>> highlighted1;
	vector<vector<Node>> highlighted2;

	Controller();
	void update(RenderWindow &window);
	void draw(RenderWindow &window);

};

//sets flagsavailable to 50, reads all the textures, calls maketiles, randomtileids
Controller::Controller()
{
	hash100->hashTable();
	srand(time(NULL));
	
	Texture holder;

	//for buttons
	holder.loadFromFile("images/selected.png"); 
	icontextures.emplace("selected", holder);
	holder.loadFromFile("images/unselected.png"); 
	icontextures.emplace("unselected", holder);

	//for shape selection
	holder.loadFromFile("images/Triangle.png"); 
	icontextures.emplace("triangle", holder);
	holder.loadFromFile("images/Rectangle.png");
	icontextures.emplace("rectangle", holder);
	holder.loadFromFile("images/Pentagon.png"); 
	icontextures.emplace("pentagon", holder);
	holder.loadFromFile("images/Hexagon.png");  
	icontextures.emplace("hexagon", holder);
	holder.loadFromFile("images/Octagon.png");  
	icontextures.emplace("octagon", holder);
	holder.loadFromFile("images/searchfor.png");
	icontextures.emplace("searchfor", holder);

	Button triangleButton = Button(icontextures, 1, 24);
	buttons.push_back(triangleButton);
	triangleword.setTexture(icontextures["triangle"]);
	triangleword.setPosition(14, 1);
	shapewords.push_back(triangleword);
	Button rectangleButton = Button(icontextures, 75, 24);
	buttons.push_back(rectangleButton);
	rectangleword.setTexture(icontextures["rectangle"]);
	rectangleword.setPosition(91, 1);
	shapewords.push_back(rectangleword);
	Button pentagonButton = Button(icontextures, 161, 24);
	buttons.push_back(pentagonButton);
	pentagonword.setTexture(icontextures["pentagon"]);
	pentagonword.setPosition(177, 1);
	shapewords.push_back(pentagonword);
	Button hexagonButton = Button(icontextures,241 , 24);
	buttons.push_back(hexagonButton);
	hexagonword.setTexture(icontextures["hexagon"]);
	hexagonword.setPosition(257, 1);
	shapewords.push_back(hexagonword);
	Button octagonButton = Button(icontextures, 321, 24);
	buttons.push_back(octagonButton);
	octagonword.setTexture(icontextures["octagon"]);
	octagonword.setPosition(335, 1);
	shapewords.push_back(octagonword);
	wordinst.setTexture(icontextures["searchfor"]);
	wordinst.setPosition(150, 1);
	shapewords.push_back(wordinst);

	//for num of node selection
	Button button100 = Button(icontextures, 450, 24);
	Button button10000 = Button(icontextures, 505, 24);
	Button button100000 = Button(icontextures, 584, 24);
	nodebutts.push_back(button100);
	nodebutts.push_back(button10000);
	nodebutts.push_back(button100000);
	holder.loadFromFile("images/100.png"); 
	icontextures.emplace("100", holder);
	holder.loadFromFile("images/10000.png"); 
	icontextures.emplace("10000", holder);
	holder.loadFromFile("images/100000.png"); 
	icontextures.emplace("100000", holder);
	holder.loadFromFile("images/numnodes.png");
	icontextures.emplace("numnodes", holder);
	word100.setTexture(icontextures["100"]);
	word100.setPosition(453, 24);
	word10000.setTexture(icontextures["10000"]);
	word10000.setPosition(516, 24);
	word100000.setTexture(icontextures["100000"]);
	word100000.setPosition(600, 24);
	nodeselect.push_back(word100);
	nodeselect.push_back(word10000);
	nodeselect.push_back(word100000);
	nodeinst.setTexture(icontextures["numnodes"]);
	nodeinst.setPosition(450, 1);
	nodeselect.push_back(nodeinst);
	nodebutts[0].toggleChosen();
	numofNodes = 1;

	//for algorithm time
	holder.loadFromFile("images/algo1.png"); //console buttons
	icontextures.emplace("algo1", holder);
	algo1.setTexture(icontextures["algo1"]);
	algo1.setPosition(700, 24);
	algowords.push_back(algo1);
	holder.loadFromFile("images/algo2.png"); //console buttons
	icontextures.emplace("algo2", holder);
	algo2.setTexture(icontextures["algo2"]);
	algo2.setPosition(1000, 24);
	algowords.push_back(algo2);
}


//checks for left and right clicks
void Controller::update(RenderWindow &window)
{
	//for the function timing I don't remember how I got it
	//I used to play around and write trading algorithms [never made any money :( ]
	//but i used this to keep track of my trades and feed them to an SQL table
	//Citation to a previous project of mine and someone else (?)
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;
	
	if (Mouse::isButtonPressed(Mouse::Left))
	{

		Vector2i position = Mouse::getPosition(window);

		if (position.y >= 23 && position.y < 41 )
		{	//These are used by the user to determine which shapes they are searching for
			if (position.x >= 1 && position.x <= 17) //triangle
			{
				for (int i = 0; i < buttons.size(); i++) {
					if (buttons[i].ischosen)
						buttons[i].toggleChosen();
				}
				highlighted.clear();
				found = false;
				buttons[0].toggleChosen();

				auto t1 = high_resolution_clock::now();
				highlighted = g100->findShapesBrute(60);
				auto t2 = high_resolution_clock::now();
				duration<double, std::milli> ms_double = t2 - t1;
				time_elapsed1 = ms_double.count();

				t1 = high_resolution_clock::now();
				hash100->searchByHash(60);
				t2 = high_resolution_clock::now();
				ms_double = t2 - t1;
				time_elapsed2 = ms_double.count();
				
				if(highlighted.size() != 0)
					found = true;
			}
			if (position.x >= 75 && position.x <= 91) //rectangle
			{
				for (int i = 0; i < buttons.size(); i++) {
					if (buttons[i].ischosen)
						buttons[i].toggleChosen();
				}
				buttons[1].toggleChosen();
				highlighted.clear();
				found = false;
				auto t1 = high_resolution_clock::now();
				highlighted = g100->findShapesBrute(90);
				auto t2 = high_resolution_clock::now();
				duration<double, std::milli> ms_double = t2 - t1;
				time_elapsed1 = ms_double.count();

				t1 = high_resolution_clock::now();
				hash100->searchByHash(90);
				t2 = high_resolution_clock::now();
				ms_double = t2 - t1;
				time_elapsed2 = ms_double.count();
				if(highlighted.size() != 0)
					found = true;
			}
			if (position.x >= 161 && position.x <= 176) //pentagon
			{
				for (int i = 0; i < buttons.size(); i++) {
					if (buttons[i].ischosen)
						buttons[i].toggleChosen();
				}
				buttons[2].toggleChosen();
				highlighted.clear();
				found = false;
				auto t1 = high_resolution_clock::now();
				highlighted = g100->findShapesBrute(108);
				auto t2 = high_resolution_clock::now();
				duration<double, std::milli> ms_double = t2 - t1;
				time_elapsed1 = ms_double.count();

				t1 = high_resolution_clock::now();
				hash100->searchByHash(108);
				t2 = high_resolution_clock::now();
				ms_double = t2 - t1;
				time_elapsed2 = ms_double.count();
				if (highlighted.size() != 0)
					found = true;
			}
			if (position.x >= 241 && position.x <= 256) //hexagon
			{
				for (int i = 0; i < buttons.size(); i++) {
					if (buttons[i].ischosen)
						buttons[i].toggleChosen();
				}
				buttons[3].toggleChosen();
				highlighted.clear();
				found = false;
				auto t1 = high_resolution_clock::now();
				highlighted = g100->findShapesBrute(120);
				auto t2 = high_resolution_clock::now();
				duration<double, std::milli> ms_double = t2 - t1;
				time_elapsed1 = ms_double.count();

				t1 = high_resolution_clock::now();
				hash100->searchByHash(120);
				t2 = high_resolution_clock::now();
				ms_double = t2 - t1;
				time_elapsed2 = ms_double.count();
				if (highlighted.size() != 0)
					found = true;
			}
			if (position.x >= 321 && position.x <= 337) //octagon
			{
				for (int i = 0; i < buttons.size(); i++) {
					if (buttons[i].ischosen)
						buttons[i].toggleChosen();
				}
				buttons[4].toggleChosen();
				highlighted.clear();
				found = false;
				auto t1 = high_resolution_clock::now();
				highlighted = g100->findShapesBrute(135);
				auto t2 = high_resolution_clock::now();
				duration<double, std::milli> ms_double = t2 - t1;
				time_elapsed1 = ms_double.count();

				t1 = high_resolution_clock::now();
				hash100->searchByHash(135);
				t2 = high_resolution_clock::now();
				ms_double = t2 - t1;
				time_elapsed2 = ms_double.count();
				if (highlighted.size() != 0)
					found = true;
			}
			if (position.x >= 450 && position.x <= 466) //100
			{//These are used to generate new graphs with the selected number of nodes
				for (int i = 0; i < nodebutts.size(); i++) { //deselects other options
					if (nodebutts[i].ischosen)
						nodebutts[i].toggleChosen();
				}
				nodebutts[0].toggleChosen();	//selects the option the user chose
				delete g100;
				g100 = new Graph(100);

			}
			if (position.x >= 505 && position.x <= 521) //10,000
			{
				for (int i = 0; i < nodebutts.size(); i++) {
					if (nodebutts[i].ischosen)
						nodebutts[i].toggleChosen();
				}
				nodebutts[1].toggleChosen();
				g100 = new Graph(10000);

			}
			if (position.x >= 584 && position.x <= 600) //100,000
			{
				for (int i = 0; i < nodebutts.size(); i++) {
					if (nodebutts[i].ischosen)
						nodebutts[i].toggleChosen();
				}
				nodebutts[2].toggleChosen();
				g100 = new Graph(100000);
			}
		}
		
	}	
	
	draw(window);
}

void Controller::draw(RenderWindow &window)
{
	vector<Node> temp;
	int x, y, x2, y2;

	if (highlighted1.size() != 1) {
		
		highlighted2.clear();
		
		//turns vector of ints highlighted1 into vector of nodes highlighted2
		for (vector<int> i : highlighted1) {
			temp.clear();
			for (int j : i) {
				temp.push_back(g100->boxList.at(j).node);
			}
			highlighted2.push_back(temp);
		}
	}


	for (int i = 0; i < g100->boxList.size(); i++) {	//used to generate all vertices and edges in the graph
		x = g100->boxList.at(i).node.x;
		y = g100->boxList.at(i).node.y + 64;
		sf::CircleShape circle(5);
		circle.setFillColor(sf::Color::Black);

		circle.setPosition(x, y);
		window.draw(circle);

	
			for (int j = 0; j < g100->edgeList[i].size(); j++) {	//generates all edges coming off the vertex
				x2 = g100->edgeList[i].at(j).x;
				y2 = g100->edgeList[i].at(j).y+64;
				sf::Vertex line[] = {
					Vertex(Vector2f(x,y)), Vertex(Vector2f(x2,y2))
				};
				line[0].color = Color::Black;
				line[1].color = Color::Black;
				window.draw(line, 2, sf::Lines);
			}
		
	}
	int x1, y1;

	//this runs if the algorithms have found a shape
	//Creates red circles and lines to be drawn over the ones made above
	if (found) {							
		for (vector<Node> solution : highlighted) {
			for (Node i : solution) {
				sf::CircleShape highlightedcircle(5);
				highlightedcircle.setFillColor(sf::Color::Red);

				highlightedcircle.setPosition(i.x, i.y+64);
				window.draw(highlightedcircle);
			}

			/*if (solution.size() == 3) {
				x1 = solution[0].x;
				y1 = solution[0].y + 64;
				x2 = solution[1].x;
				y2 = solution[1].y + 64;

				sf::Vertex line[] = {
						Vertex(Vector2f(x1,y1)), Vertex(Vector2f(x2,y2))
				};
				line[0].color = Color::Red;
				line[1].color = Color::Red;
				window.draw(line, 2, sf::Lines);

				x1 = solution[0].x;
				y1 = solution[0].y + 64;
				x2 = solution[2].x;
				y2 = solution[2].y + 64;

				Vertex line1[] = {
						Vertex(Vector2f(x1,y1)), Vertex(Vector2f(x2,y2))
				};
				line1[0].color = Color::Red;
				line1[1].color = Color::Red;
				window.draw(line1, 2, sf::Lines);

				x1 = solution[1].x;
				y1 = solution[1].y + 64;
				x2 = solution[2].x;
				y2 = solution[2].y + 64;

				Vertex line2[] = {
						Vertex(Vector2f(x1,y1)), Vertex(Vector2f(x2,y2))
				};
				line2[0].color = Color::Red;
				line2[1].color = Color::Red;
				window.draw(line2, 2, sf::Lines);

			}

			if (solution.size() == 4) {
				x1 = solution[0].x;
				y1 = solution[0].y + 64;
				x2 = solution[1].x;
				y2 = solution[1].y + 64;

				sf::Vertex line[] = {
						Vertex(Vector2f(x1,y1)), Vertex(Vector2f(x2,y2))
				};
				line[0].color = Color::Red;
				line[1].color = Color::Red;
				window.draw(line, 2, sf::Lines);

				x1 = solution[0].x;
				y1 = solution[0].y + 64;
				x2 = solution[2].x;
				y2 = solution[2].y + 64;

				Vertex line1[] = {
						Vertex(Vector2f(x1,y1)), Vertex(Vector2f(x2,y2))
				};
				line1[0].color = Color::Red;
				line1[1].color = Color::Red;
				window.draw(line1, 2, sf::Lines);

				x1 = solution[1].x;
				y1 = solution[1].y + 64;
				x2 = solution[3].x;
				y2 = solution[3].y + 64;

				Vertex line2[] = {
						Vertex(Vector2f(x1,y1)), Vertex(Vector2f(x2,y2))
				};
				line2[0].color = Color::Red;
				line2[1].color = Color::Red;
				window.draw(line2, 2, sf::Lines);

				x1 = solution[2].x;
				y1 = solution[2].y + 64;
				x2 = solution[3].x;
				y2 = solution[3].y + 64;

				Vertex line3[] = {
						Vertex(Vector2f(x1,y1)), Vertex(Vector2f(x2,y2))
				};
				line3[0].color = Color::Red;
				line3[1].color = Color::Red;
				window.draw(line3, 2, sf::Lines);

			}*/
		}
		
		algo1time = to_string(time_elapsed1);
		algo2time = to_string(time_elapsed2);

		sf::Font font; 
		font.loadFromFile("images/times-new-roman.ttf");
		Text text(algo1time, font);
		text.setCharacterSize(18);
		text.setFillColor(Color::Black);
		text.setStyle(Text::Bold);
		text.setPosition(865, 22);
		window.draw(text);
		
		Text text1(algo2time, font);
		text1.setCharacterSize(18);
		text1.setFillColor(Color::Black);
		text1.setStyle(Text::Bold);
		text1.setPosition(1165, 22);
		window.draw(text1);
		
	}
	
	//keeps all the stuff along the top visible
	for (Button i : buttons)
		i.draw(window);
	for (Sprite j : shapewords)
		window.draw(j);
	for (Button k : nodebutts)
		k.draw(window);
	for (Sprite l : nodeselect)
		window.draw(l);
	for (Sprite m : algowords)
		window.draw(m);

}