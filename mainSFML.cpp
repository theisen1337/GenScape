// opengl test.cpp : Defines the entry point for the console application.
//
#include "app.h"
#include <SFML/Graphics.hpp>
#include "infinite_map.h"
#include <iostream>
#include <string>
#include "font_overlay.h"
#include "statics_.h"
#include "app.h"

//statics_ settings;

//sf::View view;
//font_overlay font_layer;

/*
void renderDraw()
{

	sf::Text text;

	std::string test = "arial_narrow_7.ttf";

	sf::Font font;
	if (!font.loadFromFile(test))
	{
		// error...
		std::cout << "Could not load font." << std::endl;
		exit(0);
	}

	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	text.setString("0");

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::White);

	sf::RenderWindow window(sf::VideoMode(600, 600), "My window");
	//tilemap tile = tilemap(600, 600, 9, 20);

	sf::RenderTexture renderTexture;
	if (!renderTexture.create(600, 600))
	{
		// error...
	}

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		// drawing uses the same functions
		renderTexture.clear();
		//tile.render(renderTexture);
		draw_FPS(text);
		renderTexture.draw(text);
		renderTexture.display();

		// get the target texture (where the stuff has been drawn)
		const sf::Texture& texture = renderTexture.getTexture();

		// draw it to the window
		sf::Sprite sprite(texture);
		window.draw(sprite);

		// end the current frame
		window.display();
	}

}
*/
void normalDraw()
{
	//int map_sq = 11;

	//// create the window
	//sf::RenderWindow window(sf::VideoMode(settings.get_settings().get_WIN_W() , settings.get_settings().get_WIN_H()), "My window");
	//
	////create infinite map
	//infinite_map game_map(map_sq,"resources\\tilemap.png", 
	//	sf::Vector2u(settings.get_settings().get_TILE_W(), 
	//		settings.get_settings().get_TILE_W()),
	//	settings.get_settings().get_CHUNK_W(),
	//	settings.get_settings().get_CHUNK_W());

	////set view
	//view = window.getDefaultView();

	//// run the program as long as the window is open
	//while (window.isOpen())
	//{
	//	// check all the window's events that were triggered since the last iteration of the loop
	//	sf::Event event;

	//	// while there are pending events...
	//	while (window.pollEvent(event))
	//	{
	//		// check the type of the event...
	//		if (event.type == sf::Event::Closed)
	//		{
	//			window.close();
	//			break;
	//		}
	//		else if (event.type == sf::Event::MouseWheelScrolled)
	//		{
	//			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
	//				std::cout << "wheel type: vertical" << std::endl;
	//			else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
	//				std::cout << "wheel type: horizontal" << std::endl;
	//			else
	//				std::cout << "wheel type: unknown" << std::endl;
	//			std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
	//			std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
	//			std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
	//			break;
	//		}
	//		else if (event.type == sf::Event::MouseButtonPressed)
	//		{
	//			if (event.mouseButton.button == sf::Mouse::Right)
	//			{
	//				std::cout << "the right button was pressed" << std::endl;
	//				std::cout << "mouse x: " << event.mouseButton.x << std::endl;
	//				std::cout << "mouse y: " << event.mouseButton.y << std::endl;
	//			}
	//			break;
	//		}
	//		else if (event.type == sf::Event::KeyPressed)
	//		{
	//			if (sf::Keyboard::Q == event.key.code)
	//			{
	//				//game_map.check_edges();
	//				if (game_map.threadAlive == false)
	//				{
	//					game_map.threadAlive = true;
	//					game_map.call_thread();
	//				}
	//				else
	//				{
	//					game_map.threadAlive = false;
	//				}
	//				
	//			}
	//			else if (sf::Keyboard::Escape == event.key.code)
	//			{
	//				window.close();
	//			}
	//			else if (sf::Keyboard::S == event.key.code)
	//			{
	//				game_map.set_south(10.0f);
	//				//game_map.draw_thread();
	//				//game_map.call_thread();

	//			}
	//			else if (sf::Keyboard::W == event.key.code)
	//			{
	//				game_map.set_north(10.0f);
	//				//game_map.check_edges();
	//			}
	//			else if (sf::Keyboard::A == event.key.code)
	//			{
	//				game_map.set_west(10.0f);
	//				//game_map.check_edges();
	//			}
	//			else if (sf::Keyboard::D == event.key.code)
	//			{
	//				game_map.set_east(10.0f);
	//				//game_map.check_edges();
	//			}
	//			else if (sf::Keyboard::Left == event.key.code)
	//			{
	//				game_map.move_inf_map(-settings.get_settings().map_scroll_speed, 0);
	//			}
	//			else if (sf::Keyboard::Right == event.key.code)
	//			{
	//				game_map.move_inf_map(settings.get_settings().map_scroll_speed, 0);
	//			}
	//			else if (sf::Keyboard::Up == event.key.code)
	//			{
	//				game_map.move_inf_map(0, -settings.get_settings().map_scroll_speed);
	//			}
	//			else if (sf::Keyboard::Down == event.key.code)
	//			{
	//				game_map.move_inf_map(0, settings.get_settings().map_scroll_speed);
	//			}
	//			else if (sf::Keyboard::Equal == event.key.code)
	//			{
	//				// Zoom in
	//				game_map.decrease_p2p();
	//				game_map.scale_view(view);
	//				window.setView(view);
	//			}
	//			else if (sf::Keyboard::Dash == event.key.code)
	//			{
	//				// Zoom out
	//				game_map.increase_p2p();
	//				game_map.scale_view(view);
	//				window.setView(view);
	//			}
	//			break;
	//		}
	//	}


	//	// clear the window with black color
	//	window.clear(sf::Color::Black);
	//	game_map.draw(window);
	//	font_layer.draw_font(window);

	//	// end the current frame
	//	window.display();
	//}
}


void init()
{

}

int main()
{
	//init();
	//normal window object draw main loop
	//normal_draw();
	//return 0;

	app game;
	game.start();
}
