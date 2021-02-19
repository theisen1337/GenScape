#pragma once
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "types.h"

using namespace std;

class tilemap : public sf::Drawable, public sf::Transformable
{
public:
	//tilemap(int width, int height, int size, int padding_size);
	//void draw(sf::RenderWindow & window);
	//void render(sf::RenderTexture & renderer);
	//void load_tiles();

	bool load(const std::string & tileset, sf::Vector2u tileSize,
		unsigned int width, unsigned int height, int chunkx, int chunky);

	void set_scale(sf::Vector2f scale);

private:

	int get_tile_number(float map_index);

	void poly_rgb(float x, sf::Texture &t);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	sf::Texture rgb_tileset;


	const float TERRIAN_LAYERS = 4.00;

	const int GRASS_NUM = 4;
	const int SAND_NUM = 5;
	const int WATER_NUM = 4;
	const int GRASSHIGH_NUM = 4;
	const int WATERDEEP_NUM = 4;

	float deep_water_lv = -0.3;
	float water_lv = -0.1;
	float sand_lv = 0.1;
	float grass_lv = 0.3;

	float x_scale = 1;
	float y_scale = 1;
	
	int TILE_W = 48;
	int TILE_H = 48;

/*
	sf::CircleShape shape;
	vector<sf::CircleShape> shapeArr;

	int tilemap_width;
	int tilemap_height;

	const int TILESIZE_W = 48;
	const int TILESIZE_H = 48;

	sf::Texture texture;
	sf::Sprite test;
	*/
};