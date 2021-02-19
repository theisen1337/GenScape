
#include "app.h"
#include <SFML/Graphics.hpp>
#include "infinite_map.h"
#include <iostream>
#include <string>
#include "statics_.h"

statics_ settings;

app::app()
{
	std::cout << "App Started.\n";
}


app::~app()
{
	delete window_pointer;
	window_pointer = nullptr;
	std::cout << "App Closed.\n";
}


void app::_init()
{
	int map_sq = 11;
	// create the window
	window_pointer = new sf::RenderWindow(sf::VideoMode(settings.get_settings().get_WIN_W(), settings.get_settings().get_WIN_H()), "My window",sf::Style::Default);
	//window_pointer = new sf::RenderWindow(sf::VideoMode().getFullscreenModes(), "My window");
	//game_map = infinite_map();

	if (!render_texture.create(settings.get_settings().get_TILE_W() * 
		settings.get_settings().get_CHUNK_W() *
		map_sq,
		settings.get_settings().get_TILE_H() *
		settings.get_settings().get_CHUNK_H() *
		map_sq))
	{
		std::cout << "error with creating render_texture\n";
		exit(1);
	}
	else
	{
		std::cout << "render texture loaded.\n";
	}
	//render_texture.create(600, 600);
	// Optimization
	// #------------------------------------------------------------------------------#
	// In hopes of removing screen tearing
	//window_pointer->setVerticalSyncEnabled(true);
	// #------------------------------------------------------------------------------#

	//create infinite map
	game_map.init(map_sq, "resources\\tilemap.png",
		sf::Vector2u(settings.get_settings().get_TILE_W(),
			settings.get_settings().get_TILE_W()),
		settings.get_settings().get_CHUNK_W(),
		settings.get_settings().get_CHUNK_W());

	//set view
	view = window_pointer->getDefaultView();
	default_view = window_pointer->getDefaultView();
}


void app::_controls()
{
	// while there are pending events...
	while (window_pointer->pollEvent(event))
	{
		// check the type of the event...
		if (event.type == sf::Event::Closed)
		{
			window_pointer->close();
			break;
		}
		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				std::cout << "wheel type: vertical" << std::endl;
			else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
				std::cout << "wheel type: horizontal" << std::endl;
			else
				std::cout << "wheel type: unknown" << std::endl;
			std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
			std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
			std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
			break;
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "the right button was pressed" << std::endl;
				std::cout << "mouse x: " << event.mouseButton.x << std::endl;
				std::cout << "mouse y: " << event.mouseButton.y << std::endl;
			}
			break;
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (sf::Keyboard::Q == event.key.code)
			{
				//game_map.check_edges();
				if (game_map.threadAlive == false)
				{
					game_map.threadAlive = true;
					game_map.call_thread();
				}
				else
				{
					game_map.threadAlive = false;
				}

			}
			else if (sf::Keyboard::Escape == event.key.code)
			{
				window_pointer->close();
			}
			else if (sf::Keyboard::S == event.key.code)
			{
				game_map.set_south(10.0f);
				//game_map.draw_thread();
				//game_map.call_thread();

			}
			else if (sf::Keyboard::W == event.key.code)
			{
				game_map.set_north(10.0f);
				//game_map.check_edges();
			}
			else if (sf::Keyboard::A == event.key.code)
			{
				game_map.set_west(10.0f);
				//game_map.check_edges();
			}
			else if (sf::Keyboard::D == event.key.code)
			{
				game_map.set_east(10.0f);
				//game_map.check_edges();
			}
			else if (sf::Keyboard::Left == event.key.code)
			{
				//game_map.move_inf_map(-settings.get_settings().map_scroll_speed, 0);
				view.move(-1.0f, 0.0f);
				window_pointer->setView(view);
			}
			else if (sf::Keyboard::Right == event.key.code)
			{
				//game_map.move_inf_map(settings.get_settings().map_scroll_speed, 0);
				view.move(1.0f, 0.0f);
				window_pointer->setView(view);
			}
			else if (sf::Keyboard::Up == event.key.code)
			{
				//game_map.move_inf_map(0, -settings.get_settings().map_scroll_speed);
				view.move(0.0f, 1.0f);
				window_pointer->setView(view);
			}
			else if (sf::Keyboard::Down == event.key.code)
			{
				//game_map.move_inf_map(0, settings.get_settings().map_scroll_speed);
				view.move(0.0f,-1.0f);
				window_pointer->setView(view);

			}
			else if (sf::Keyboard::Equal == event.key.code)
			{
				// Zoom in
				//game_map.decrease_p2p();
				//game_map.scale_view(view);
				//window_pointer->setView(view);

				view.zoom(0.75f);
				window_pointer->setView(view);
			}
			else if (sf::Keyboard::Dash == event.key.code)
			{
				// Zoom out
				//game_map.increase_p2p();
				//game_map.scale_view(view);
				//window_pointer->setView(view);
				view.zoom(1.25f);
				window_pointer->setView(view);
			}
			break;
		}
	}
}


void app::_normal_draw()
{
	// run the program as long as the window is open
	while (window_pointer->isOpen())
	{
		// check keyboard and mouse input.
		_controls();
		
		// clear the window with black color
		//window_pointer->clear();
		//game_map.draw(window_pointer);
		//font_layer.draw_font(window_pointer);

		// end the current frame
		//window_pointer->display();

		
		/*-----------------------------------------------------------*/
		// Draw the texture
		//render_texture.clear();
		game_map.draw_render(render_texture);
		render_texture.display();

		//// get the target texture (where the stuff has been drawn)
		const sf::Texture& texture = render_texture.getTexture();

		sf::Sprite sprite(render_texture.getTexture());
		window_pointer->clear();
		window_pointer->draw(sprite);
		font_layer.draw_font(window_pointer);
		window_pointer->display();
	    /*-----------------------------------------------------------*/
	}
}

int app::start()
{
	_init();
	_normal_draw();
	return 0;
}