#pragma once
#include "infinite_map.h"
#include "font_overlay.h"

class app
{
public:
	app();
	~app();
	int start();

private:
	
	void _normal_draw();
	void _init();

	void _controls();

	// check all the window's events that were triggered since the last iteration of the loop
	sf::Event event;
	sf::RenderTexture render_texture;
	sf::View view;
	sf::View default_view;
	font_overlay font_layer;
	infinite_map game_map;
	sf::RenderWindow *window_pointer;
};