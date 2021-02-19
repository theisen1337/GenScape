#pragma once
#include "tilemap.h"
#include <SFML/Graphics.hpp>
#include "statics_.h"
#include "types.h"




struct chunk {
	
	tilemap chunk_map;
	float chunk_posx_f;
	float chunk_posy_f;
	int chunk_pos_x;
	int chunk_pos_y;
	//bool load = false;
	//bool draw = false;
	int chunk_current_width;
	int chunk_current_height;


	bool is_center = false;
	edge chunk_edge;

	int chunk_x;
	int chunk_y;

	int chunk_width;
	int chunk_height;

	/*
	chunk(const chunk& d) :
		chunk_map(d.chunk_map),
		posx_f(d.posx_f),
		posy_f(d.posy_f),
		posx(d.posx),
		posy(d.posy),
		load(d.load),
		draw(d.draw),
		current_w(d.current_w),
		current_h(d.current_h),
		is_center(d.is_center),
		chunk_edge(d.chunk_edge),
		__cx(d.__cx),
		__cy(d.__cy),
		__cw(d.__cw),
		__ch(d.__ch)
		*/

};

class infinite_map
{
public:
	infinite_map();
	~infinite_map();

	void init(int sq, std::string tileset, sf::Vector2u tileSize, unsigned int chunk_w, unsigned int chunk_h);

	void scale(sf::Vector2f scale);
	void draw(sf::RenderWindow * window);
	void draw_render(sf::RenderTexture& rt);
	void pixel_perfect_scale(sf::Vector2f scale);
	void set_scale(sf::Vector2f s);

	void increase_p2p();
	void decrease_p2p();
	void call_thread();
	void scale_view(sf::View & v);

	void set_east(float e) { map_east_pos += e; }
	void set_west(float w) { map_west_pos += w; }
	void set_north(float n) { map_north_pos += n; }
	void set_south(float s) { map_south_pos += s; }

	void check_edges();
	void move_inf_map(int x, int y);

	bool threadAlive = false;

	
private:
	sf::Color poly_rgb(float x);
	bool check_center(int cx, int cy, int cw, int ch, int mw, int mh);
	void thread_safe_delete(int i);
	void thread_safe_draw(sf::RenderWindow * window, int i);
	void thread_safe_draw_render(sf::RenderTexture& re, int i);
	void scale_by_pixel();

	int test_count = 0;
	void thread_loop();

	// Edge expansion
	void load_edge();
	void update_map();
	sf::Thread t_draw;

	sf::RectangleShape r;
	std::vector<chunk*> inf_map;
	std::vector<chunk*> East_map;
	std::vector<chunk*> West_map;
	std::vector<chunk*> North_map;
	std::vector<chunk*> South_map;

	bool t_running = false;

	float map_pixel_to_pixel;
	int map_center_index;

	
	int map_original_height;
	int map_original_width;
	int map_current_width;
	int map_current_height;
	sf::Vector2u map_tile_size;

	int map_max_chunks;
	int map_sq;
	int map_chunk_width;
	int map_chunk_height;
	std::string map_tileset_str;
	statics_ settings;

	bool map_EastLoaded = false;
	bool map_WestLoaded = false;
	bool map_NorthLoaded = false;
	bool map_SouthLoaded = false;

	float map_east_pos = 0;
	float map_west_pos = 0;
	float map_north_pos = 0;
	float map_south_pos = 0;
	edge map_expand_edge = NO_EDGE;

	sf::Mutex mutex; // for locking threads

};

