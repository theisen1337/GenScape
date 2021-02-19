#pragma once
#include <SFML/Graphics.hpp>

class fps
{
public:
	fps();
	~fps();
	void draw_FPS(sf::Text & t);
private:
	
	int FPS = 0;
	sf::Clock clock1;

};

