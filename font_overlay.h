#pragma once
#include <SFML/Graphics.hpp>
#include "fps.h"

class font_overlay
{
public:
	font_overlay();
	~font_overlay();

	void add_font(std::string t, int fontsize, int posx, int posy);
	void draw_font(sf::RenderWindow * w);


private:
	sf::Text text;
	sf::Font font;
	fps framerate;
	std::vector<sf::Text> overlay;
};

