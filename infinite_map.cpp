#include "infinite_map.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>


infinite_map::infinite_map() : t_draw(&infinite_map::thread_loop, this)
{

}

void infinite_map::init(int sq, std::string tileset, sf::Vector2u tileSize, unsigned int chunk_w,unsigned int chunk_h)
{
	if (sq % 2 == 0)
	{
		std::cout << "Error chunk amount must be sqaure odd number 1,9,25,49,.." << std::endl;
		exit(1);
	}
	
	/* Setup class parameters */
	infinite_map::map_max_chunks		= sq * sq;
	infinite_map::map_sq				= sq;
	infinite_map::map_current_height	= chunk_h * sq * tileSize.y;
	infinite_map::map_current_width		= chunk_w * sq * tileSize.x;
	infinite_map::map_original_height	= map_current_height;
	infinite_map::map_original_width	= map_current_width;
	infinite_map::map_tile_size			= tileSize;
	infinite_map::map_tileset_str		= tileset;
	infinite_map::map_chunk_height		= chunk_h;
	infinite_map::map_chunk_width		= chunk_w;
	infinite_map::map_pixel_to_pixel	= int(settings.get_settings().get_WIN_W() / (float)(chunk_w * sq)); // <-- fit entire map in window
	//infinite_map::map_pixel_to_pixel	= 10;

	// Initialize Edge Chunk maps
	for (unsigned int i = 0; i < infinite_map::map_sq; i++)
	{
		East_map.push_back(nullptr);
		West_map.push_back(nullptr);
		North_map.push_back(nullptr);
		South_map.push_back(nullptr);
	}

	// Initialize chunks
	for (int i = 0; i < infinite_map::map_max_chunks; i++)
	{
		inf_map.push_back(nullptr);
		inf_map.at(i) = new chunk();
	}

	for (int x = 0; x < sq; x++)
	{
		for (int y = 0; y < sq; y++)
		{
			//chunk x and y initial x,y
			int chunkx = (x * tileSize.x * chunk_w);
			int chunky = (y * tileSize.y * chunk_h);

			// Space out the chunks, so each one does not over lap, the chunks will need to be respaced when scaled.
			inf_map.at(x + y * sq)->chunk_pos_x				= chunkx;
			inf_map.at(x + y * sq)->chunk_pos_y				= chunky;
			inf_map.at(x + y * sq)->chunk_x					= chunkx;
			inf_map.at(x + y * sq)->chunk_y					= chunky;
			inf_map.at(x + y * sq)->chunk_width				= tileSize.x * chunk_w;
			inf_map.at(x + y * sq)->chunk_height			= tileSize.y * chunk_h;
			inf_map.at(x + y * sq)->chunk_current_width		= tileSize.x * chunk_w;
			inf_map.at(x + y * sq)->chunk_current_height	= tileSize.y * chunk_h;


			// Generate the textures, and noise 
			if (!inf_map.at(x + y * sq)->chunk_map.load(tileset, tileSize, chunk_w, chunk_h, chunkx ,chunky))
			{
				// if there is a failure in loading the map.
				std::cout << "failed to load map." << std::endl;
				exit(1);
			}
			
			// Check for center chunk, and set center index
			if (check_center(inf_map.at(x + y * sq)->chunk_x, inf_map.at(x + y * sq)->chunk_y,
				inf_map.at(x + y * sq)->chunk_width, inf_map.at(x + y * sq)->chunk_height, infinite_map::map_current_width, infinite_map::map_current_height))
			{
				inf_map.at(x + y * sq)->is_center = true;
				infinite_map::map_center_index = x + y * sq;
			}
		}
	}

	// Scale to default pixel
	//TODO screws rendertexture
	scale_by_pixel();

	// Set Center chunk outline.
	r.setSize(sf::Vector2f(inf_map.at(map_center_index)->chunk_current_width, inf_map.at(map_center_index)->chunk_current_height));
	r.setPosition(inf_map.at(map_center_index)->chunk_pos_x, inf_map.at(map_center_index)->chunk_pos_y);
	r.setFillColor(sf::Color(0, 0, 0, 0));
	r.setOutlineThickness(2);
	r.setOutlineColor(sf::Color(150, 150, 50));

}
 

void infinite_map::load_edge()
{
	float scale = 3.0 / (float)map_tile_size.x;//map_pixel_to_pixel / (float)map_tile_size.x;
	sf::Vector2f new_scale		= sf::Vector2f(scale, scale);

	if(map_expand_edge == EAST && !map_EastLoaded)
	{
		map_EastLoaded	= true;
		int index		= (map_sq - 1) + 0 * map_sq;

		int E_chunkx	= infinite_map::inf_map.at(index)->chunk_x;
		int E_chunky	= infinite_map::inf_map.at(index)->chunk_y;
		int E_posx		= infinite_map::inf_map.at(index)->chunk_pos_x;
		int E_posy		= infinite_map::inf_map.at(index)->chunk_pos_y;

		for (unsigned int i = 0; i < map_sq; i++)
		{
			East_map.at(i)			= new chunk();
			East_map.at(i)->chunk_map.load(
				infinite_map::map_tileset_str,
				infinite_map::map_tile_size, 
				infinite_map::map_chunk_width,
				infinite_map::map_chunk_height,
				E_chunkx + (infinite_map::map_tile_size.x * infinite_map::map_chunk_width),
				E_chunky + (i * infinite_map::map_tile_size.y * infinite_map::map_chunk_height));

			East_map.at(i)->chunk_pos_x			= E_posx + (map_tile_size.x * map_chunk_width) * new_scale.x;
			East_map.at(i)->chunk_pos_y			= E_posy + (i * map_tile_size.y * map_chunk_height) * new_scale.y;

			East_map.at(i)->chunk_x = E_chunkx + (map_tile_size.x * map_chunk_width);
			East_map.at(i)->chunk_y = E_chunky + (i * map_tile_size.y * map_chunk_height);

			East_map.at(i)->chunk_width = map_tile_size.x * map_chunk_width;
			East_map.at(i)->chunk_height = map_tile_size.y * map_chunk_height;

			East_map.at(i)->chunk_current_width = map_tile_size.x * map_chunk_width;
			East_map.at(i)->chunk_current_height = map_tile_size.y * map_chunk_height;
			
			// Set scale, of map chunk edge.
			East_map.at(i)->chunk_map.setScale(new_scale);

			East_map.at(i)->chunk_map.setPosition(East_map.at(i)->chunk_pos_x, East_map.at(i)->chunk_pos_y);
			East_map.at(i)->chunk_current_width = inf_map.at(i)->chunk_width * new_scale.x;
			East_map.at(i)->chunk_current_height = inf_map.at(i)->chunk_height * new_scale.y;

		}
		std::cout << "loaded East Wall."  << std::endl;
	}
	else if(map_expand_edge == WEST && !map_WestLoaded)
	{
		map_WestLoaded	= true;
		int W_chunkx	= infinite_map::inf_map.at(0)->chunk_x;
		int W_chunky	= infinite_map::inf_map.at(0)->chunk_y;
		for (unsigned int i = 0; i < map_sq; i++)
		{

			West_map.at(i) = new chunk();
			int t1 = W_chunkx - (map_tile_size.x * map_chunk_width);
			int t2 = W_chunky + (i * map_tile_size.y * map_chunk_height);
			West_map.at(i)->chunk_map.load(map_tileset_str,
				map_tile_size,
				map_chunk_width,
				map_chunk_height,
				W_chunkx - (map_tile_size.x * map_chunk_width),
				W_chunky + (i * map_tile_size.y * map_chunk_height));

			West_map.at(i)->chunk_pos_x				= W_chunkx - (map_tile_size.x * map_chunk_width);
			West_map.at(i)->chunk_pos_y				= W_chunky + (i * map_tile_size.y * map_chunk_height);
			
			West_map.at(i)->chunk_x					= W_chunkx - (map_tile_size.x * map_chunk_width);
			West_map.at(i)->chunk_y					= W_chunky + (i * map_tile_size.y * map_chunk_height);
			
			West_map.at(i)->chunk_width				= map_tile_size.x * map_chunk_width;
			West_map.at(i)->chunk_height			= map_tile_size.y * map_chunk_height;
			
			West_map.at(i)->chunk_current_width		= map_tile_size.x * map_chunk_width;
			West_map.at(i)->chunk_current_height	= map_tile_size.y * map_chunk_height;


			West_map.at(i)->chunk_map.setScale(new_scale);

			int tx = West_map.at(i)->chunk_x * new_scale.x;
			int ty = West_map.at(i)->chunk_y * new_scale.y;

			West_map.at(i)->chunk_map.setPosition(tx, ty);
			West_map.at(i)->chunk_pos_x = tx;
			West_map.at(i)->chunk_pos_y = ty;
			West_map.at(i)->chunk_current_width = inf_map.at(i)->chunk_width * new_scale.x;
			West_map.at(i)->chunk_current_height = inf_map.at(i)->chunk_height * new_scale.y;

		}
		std::cout << "loaded West Wall." << std::endl;
	}
	else if (map_expand_edge == NORTH && !map_NorthLoaded)
	{
		map_NorthLoaded = true;
		int N_chunkx = infinite_map::inf_map.at(0)->chunk_x;
		int N_chunky = infinite_map::inf_map.at(0)->chunk_y;
		for (unsigned int i = 0; i < map_sq; i++)
		{
			// create object
			North_map.at(i) = new chunk();

			// load chunk for index
			North_map.at(i)->chunk_map.load(map_tileset_str, map_tile_size, map_chunk_width, map_chunk_height,
				N_chunkx + (i * map_tile_size.x * map_chunk_width), N_chunky - (map_tile_size.y * map_chunk_height));

			// set initial settings
			North_map.at(i)->chunk_pos_x = N_chunkx + (i * map_tile_size.x * map_chunk_width);
			North_map.at(i)->chunk_pos_y = N_chunky - (map_tile_size.y * map_chunk_height);
			North_map.at(i)->chunk_x = N_chunkx + (i * map_tile_size.x * map_chunk_width);
			North_map.at(i)->chunk_y = N_chunky - (map_tile_size.y * map_chunk_height);
			North_map.at(i)->chunk_width = map_tile_size.x * map_chunk_width;
			North_map.at(i)->chunk_height = map_tile_size.y * map_chunk_height;
			North_map.at(i)->chunk_current_width = map_tile_size.x * map_chunk_width;
			North_map.at(i)->chunk_current_height = map_tile_size.y * map_chunk_height;

			// Set scale for chunk
			North_map.at(i)->chunk_map.setScale(new_scale);

			// Set more initial settings
			int tx = North_map.at(i)->chunk_x * new_scale.x;
			int ty = North_map.at(i)->chunk_y * new_scale.y;

			North_map.at(i)->chunk_map.setPosition(tx, ty);
			North_map.at(i)->chunk_pos_x = tx;
			North_map.at(i)->chunk_pos_y = ty;
			North_map.at(i)->chunk_current_width = inf_map.at(i)->chunk_width * new_scale.x;
			North_map.at(i)->chunk_current_height = inf_map.at(i)->chunk_height * new_scale.y;

		}
		std::cout << "loaded North Wall." << std::endl;
	}
	else if (map_expand_edge == SOUTH && !map_SouthLoaded)
	{
		map_SouthLoaded = true;
		int S_chunkx = infinite_map::inf_map.at(map_max_chunks - map_sq)->chunk_x;
		int S_chunky = infinite_map::inf_map.at(map_max_chunks - map_sq)->chunk_y;
		for (unsigned int i = 0; i < map_sq; i++)
		{
			// create object
			South_map.at(i) = new chunk();

			// load chunk for index
			South_map.at(i)->chunk_map.load(map_tileset_str,
				map_tile_size,
				map_chunk_width,
				map_chunk_height,
				S_chunkx + (i * map_tile_size.x * map_chunk_width),
				S_chunky + (map_tile_size.y * map_chunk_height));

			// set initial settings
			South_map.at(i)->chunk_pos_x	= S_chunkx + (i * map_tile_size.x * map_chunk_width);
			South_map.at(i)->chunk_pos_y	= S_chunky + (map_tile_size.y * map_chunk_height);
			South_map.at(i)->chunk_x		= S_chunkx + (i * map_tile_size.x * map_chunk_width);
			South_map.at(i)->chunk_y		= S_chunky + (map_tile_size.y * map_chunk_height);

			South_map.at(i)->chunk_width			= map_tile_size.x * map_chunk_width;
			South_map.at(i)->chunk_height			= map_tile_size.y * map_chunk_height;
			South_map.at(i)->chunk_current_width	= map_tile_size.x * map_chunk_width;
			South_map.at(i)->chunk_current_height	= map_tile_size.y * map_chunk_height;

			// Set scale for chunk
			South_map.at(i)->chunk_map.setScale(new_scale);

			// Set more initial settings
			int tx = South_map.at(i)->chunk_x * new_scale.x;
			int ty = South_map.at(i)->chunk_y * new_scale.y;

			South_map.at(i)->chunk_map.setPosition(tx, ty);
			South_map.at(i)->chunk_pos_x = tx;
			South_map.at(i)->chunk_pos_y = ty;
			South_map.at(i)->chunk_current_width = inf_map.at(i)->chunk_width * new_scale.x;
			South_map.at(i)->chunk_current_height = inf_map.at(i)->chunk_height * new_scale.y;

		}
		std::cout << "loaded South Wall." << std::endl;
	}
}


void infinite_map::thread_safe_delete(int i)
{
	sf::Lock lock(mutex);
	delete inf_map[i];
	inf_map[i] = nullptr;
}


void infinite_map::update_map()
{
	if (map_expand_edge == EAST && map_EastLoaded)
	{
		for (unsigned int i = 0; i < map_sq; i++)
		{
			thread_safe_delete(i * map_sq);
		}

		for (int x = 0; x < map_sq - 1; x++)
		{
			for (int y = 0; y < map_sq; y++)
			{
				// Set pointer from the right to the left.
				inf_map.at(x + y * map_sq) = inf_map.at((x + 1) + y * map_sq);
			}
		}

		for (unsigned int i = 0; i < map_sq; i++)
		{
			// Set pointer from the right to the left.
			inf_map.at((map_sq - 1) + i * map_sq) = East_map.at(i);
		}
		map_EastLoaded = false;
	}
	else if (map_expand_edge == WEST && map_WestLoaded)
	{
		for (unsigned int i = 0; i < map_sq; i++)
		{
			thread_safe_delete(((map_sq - 1) + i * map_sq));
		}

		for (int x = 0; x < map_sq - 1; x++)
		{
			for (int y = 0; y < map_sq; y++)
			{
				// Set pointer from the left to the right.
				inf_map.at(((map_sq - 1) - x) + y * map_sq) = inf_map.at(((map_sq - 1) - (x + 1)) + y * map_sq);
			}
		}

		for (unsigned int i = 0; i < map_sq; i++)
		{
			// Set pointer from the left to the right.
			inf_map.at(0 + i * map_sq) = West_map[i];
		}
		map_WestLoaded = false;
	}
	else if (map_expand_edge == NORTH && map_NorthLoaded)
	{
		for (unsigned int i = 0; i < map_sq; i++)
		{
			thread_safe_delete((map_max_chunks - 1) - i);
		}

		for (int x = 0; x < map_sq - 1; x++)
		{
			for (int y = 0; y < map_sq; y++)
			{
				// Set pointer from the North to the South.
				int t = ((map_sq - x) * map_sq) - (map_sq - y);
				int f = ((map_sq - (x+1)) * map_sq) - (map_sq - y);
				inf_map.at(t) = inf_map.at(f);
			}
		}

		for (unsigned int i = 0; i < map_sq; i++)
		{
			// Set pointer from the north to the south.
			inf_map.at(i) = North_map[i];
		}
		map_NorthLoaded = false;
	}
	else if (map_expand_edge == SOUTH && map_SouthLoaded)
	{
		for (unsigned int i = 0; i < map_sq; i++)
		{
			thread_safe_delete(i);
		}

		for (int x = 0; x < map_sq; x++)
		{
			for (int y = 0; y < map_sq - 1; y++)
			{
				// Set pointer from the North to the South.
				int t = x + y * map_sq;
				int f = x + (y+1) * map_sq;
				inf_map.at(t) = inf_map.at(f);
			}
		}

		for (unsigned int i = 0; i < map_sq; i++)
		{
			// Set pointer from the north to the south.
			inf_map.at((map_max_chunks - 1) - i) = South_map[(map_sq-1) - i];
		}
		map_SouthLoaded = false;
	}

	r.setSize(sf::Vector2f(inf_map.at(map_center_index)->chunk_current_width,
		inf_map.at(map_center_index)->chunk_current_height));
	r.setPosition(inf_map.at(map_center_index)->chunk_pos_x,
		inf_map.at(map_center_index)->chunk_pos_y);
}


infinite_map::~infinite_map()
{

	for (std::vector< chunk* >::iterator c_iter = inf_map.begin(); c_iter != inf_map.end(); ++c_iter)
	{
		delete (*c_iter);
	}
	inf_map.clear();

	std::cout << "Infinite map dealocate" << std::endl;
}


void infinite_map::thread_safe_draw(sf::RenderWindow * window, int i)
{
	sf::Lock lock(mutex);
	if (inf_map.at(i) != nullptr)
	{
		window->draw(inf_map.at(i)->chunk_map);
	}
	else
	{
		std::cout << "Nullpter"<<std::endl;
	}
}


void infinite_map::thread_safe_draw_render(sf::RenderTexture &re, int i)
{
	sf::Lock lock(mutex);
	if (inf_map.at(i) != nullptr)
	{
		re.draw(inf_map.at(i)->chunk_map);
	}
	else
	{
		std::cout << "Nullpter" << std::endl;
	}
}


void infinite_map::draw(sf::RenderWindow * window)
{
	for (int x = 0; x < map_sq; x++)
	{
		for (int y = 0; y < map_sq; y++)
		{
			thread_safe_draw(window,x + y * map_sq);
		}
	}
	// Draw centerbox
	window->draw(r);
}

void infinite_map::draw_render(sf::RenderTexture &rt)
{
	for (int x = 0; x < map_sq; x++)
	{
		for (int y = 0; y < map_sq; y++)
		{
			thread_safe_draw_render(rt, x + y * map_sq);
		}
	}
	// Draw centerbox
	rt.draw(r);
}


bool infinite_map::check_center(int cx, int cy, int cw, int ch, int mw, int mh)
{
	return (mw /2) - (cw / 2) == cx && (mh / 2) - (ch / 2) == cy;
}


void infinite_map::thread_loop()
{
	while (infinite_map::threadAlive)
	{
		check_edges();
	}
}


void infinite_map::check_edges()
{

	if (map_south_pos >= 10.0)
	{
		map_south_pos = 0;
		map_expand_edge = SOUTH;
	}
	else if (map_north_pos >= 10.0)
	{
		map_north_pos = 0;
		map_expand_edge = NORTH;
	}
	else if (map_west_pos >= 10.0)
	{
		map_west_pos = 0;
		map_expand_edge = WEST;
	}
	else if (map_east_pos >= 10.0)
	{
		map_east_pos = 0;
		map_expand_edge = EAST;
	}
	else
	{
		return;
	}
	
	load_edge();
	update_map();
	map_expand_edge = NO_EDGE;
	t_running = false;
}


void infinite_map::move_inf_map(int offset_x, int offset_y)
{
	for (int x = 0; x < map_sq; x++)
	{
		for (int y = 0; y < map_sq; y++)
		{
			inf_map.at(x + y * map_sq)->chunk_map.move(offset_x, offset_y);
			inf_map.at(x + y * map_sq)->chunk_pos_x += offset_x;
			inf_map.at(x + y * map_sq)->chunk_pos_y += offset_y;
		}
	}
	// Set Size.
	r.setSize(sf::Vector2f(inf_map.at(map_center_index)->chunk_current_width,
		inf_map.at(map_center_index)->chunk_current_height));

	// Set Position.
	r.setPosition(inf_map.at(map_center_index)->chunk_pos_x,
		inf_map.at(map_center_index)->chunk_pos_y);
}







void infinite_map::increase_p2p()
{
	map_pixel_to_pixel+=2;
}


void infinite_map::decrease_p2p()
{
	if (map_pixel_to_pixel > 3)
	{
		map_pixel_to_pixel-=2;
	}
}


void infinite_map::call_thread()
{
	if (!t_running)
	{
		t_running = true;
		t_draw.launch();
	}
}

// used
void infinite_map::scale_view(sf::View & v)
{
	float scale = map_pixel_to_pixel / (float)map_tile_size.x;
	std::cout << "p2p: " << map_pixel_to_pixel << "\t" << (map_tile_size.x * map_tile_size.x) / (float)map_pixel_to_pixel << std::endl;
	v.setSize(settings.get_settings().get_WIN_W() * scale, settings.get_settings().get_WIN_H() * scale);
}


/*
sf::Color infinite_map::poly_rgb(float x)
{
	sf::Uint8 * color = new sf::Uint8[4];

	// Color Map
	color[0] = -770.00 * pow(x, 2) + 21.00 * x + 225.00;
	color[1] = -502.00 * pow(x, 2) + 145.00 * x + 251.00;
	color[2] = -12.00 * pow(x, 2) + -178.00 * x + 140.00;
	color[3] = 255;

	sf::Color test(color[0], color[1], color[2]);
	return test;
}
*/
//used
void infinite_map::set_scale(sf::Vector2f s)
{

	for (int i = 0; i < map_max_chunks; i++)
	{
		inf_map.at(i)->chunk_map.setScale(s);

		int tx = inf_map.at(i)->chunk_x * s.x;
		int ty = inf_map.at(i)->chunk_y * s.y;

		inf_map.at(i)->chunk_map.setPosition(tx, ty);
		inf_map.at(i)->chunk_pos_x = tx;
		inf_map.at(i)->chunk_pos_y = ty;
		inf_map.at(i)->chunk_current_width = inf_map.at(i)->chunk_width * s.x;
		inf_map.at(i)->chunk_current_height = inf_map.at(i)->chunk_height * s.y;
	}

	infinite_map::map_current_height = map_original_height * s.x;
	infinite_map::map_current_width = map_original_width * s.y;

}

/*
void infinite_map::scale(sf::Vector2f scale)
{
	for (int i = 0; i < map_max_chunks; i++)
	{
		inf_map.at(i)->chunk_map.scale(scale);

		int tx = inf_map.at(i)->chunk_pos_x * scale.x;
		int ty = inf_map.at(i)->chunk_pos_y * scale.y;

		inf_map.at(i)->chunk_map.setPosition(tx, ty);
		inf_map.at(i)->chunk_pos_x = tx;
		inf_map.at(i)->chunk_pos_y = ty;
		inf_map.at(i)->chunk_current_width = inf_map.at(i)->chunk_current_width * scale.x;
		inf_map.at(i)->chunk_current_height = inf_map.at(i)->chunk_current_height * scale.y;
	}

	infinite_map::map_current_height = map_original_height * scale.x;
	infinite_map::map_current_width = map_original_width * scale.y;
}
*/
//used
void infinite_map::scale_by_pixel()
{

	float scale = map_pixel_to_pixel / (float)map_tile_size.x;
	//float scale = map_tile_size.x / (float)map_pixel_to_pixel;
	std::cout << "p2p: " << map_pixel_to_pixel << "\t" << (map_tile_size.x * map_tile_size.x) / (float)map_pixel_to_pixel << std::endl;

	infinite_map::set_scale(sf::Vector2f(scale, scale));

	r.setSize(sf::Vector2f(inf_map.at(map_center_index)->chunk_current_width,
		inf_map.at(map_center_index)->chunk_current_height));
	r.setPosition(inf_map.at(map_center_index)->chunk_pos_x,
		inf_map.at(map_center_index)->chunk_pos_y);

}

/*
void infinite_map::pixel_perfect_scale(sf::Vector2f scale)
{
	float new_pixel_x = scale.x * float(map_tile_size.x);
	float new_pixel_y = scale.y * float(map_tile_size.y);

	int floor_pixel_x = (int)new_pixel_x;
	int floor_pixel_y = (int)new_pixel_y;


	float new_scaleX = floor_pixel_x / (float)map_tile_size.x;
	float new_scaleY = floor_pixel_y / (float)map_tile_size.y;
	infinite_map::scale(sf::Vector2f(new_scaleX, new_scaleY));
}
*/