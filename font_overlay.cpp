#include "font_overlay.h"
#include "fps.h"
#include <iostream>


font_overlay::font_overlay()
{
	std::string test = "arial_narrow_7.ttf";

	// Attempt to load font, if file doesn't exist error
	if (!font.loadFromFile(test))
	{
		// error...
		std::cout << "Could not load font." << std::endl;
		exit(0);
	}
	else
	{
		std::cout << "Load font: " << test << std::endl;
	}

	// select the font, font is a sf::Font
	text.setFont(font);

	// set the string to display
	text.setString("");

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::White);

	// fps
	framerate = fps();
	add_font("", 24, 5, 5);

}

void font_overlay::add_font(std::string t, int fontsize, int posx, int posy)
{
	overlay.push_back(sf::Text());
	overlay.back().setFont(font);
	overlay.back().setString(t);
	overlay.back().setCharacterSize(fontsize);
	overlay.back().setFillColor(sf::Color::White);
	overlay.back().setPosition(sf::Vector2f(posx, posy));
}

void font_overlay::draw_font(sf::RenderWindow *w)
{
	

	if (overlay.size() > 0)
	{
		font_overlay::framerate.draw_FPS(overlay.at(0));

		for (int i = 0; i < overlay.size(); i++)
			w->draw(overlay.at(i));
	}
}

font_overlay::~font_overlay()
{
}
