#include <SFML/Graphics.hpp>
#include "tilemap.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include "generate_map.h"


int tilemap::get_tile_number(float map_index)
{
	if (map_index < deep_water_lv)
	{
		return rand() % WATERDEEP_NUM + GRASSHIGH_NUM + GRASS_NUM + SAND_NUM + WATER_NUM;
	}
	else if (map_index < water_lv)
	{
		return rand() % WATER_NUM + GRASS_NUM + SAND_NUM;
	}
	else if (map_index < sand_lv)
	{
		return rand() % SAND_NUM + GRASS_NUM;
	}
	else if (map_index < grass_lv)
	{
		return rand() % GRASS_NUM;
	}
	else
	{
		return rand() % GRASSHIGH_NUM + GRASS_NUM + SAND_NUM + WATER_NUM;
	}
}


void tilemap::poly_rgb(float x, sf::Texture &t)
{
	sf::Uint8 * color = new sf::Uint8[4];

	// Color Map
	color[0] = -770.00 * pow(x, 2) + 21.00 * x + 225.00;
	color[1] = -502.00 * pow(x, 2) + 145.00 * x + 251.00;
	color[2] = -12.00 * pow(x, 2) + -178.00 * x + 140.00;
	color[3] = 255;

	t.update(color);
}


bool tilemap::load(const std::string & tileset, sf::Vector2u tileSize,
	unsigned int width, unsigned int height,int chunkx, int chunky)
{
	// load the tileset texture
	if (!tilemap::m_tileset.loadFromFile(tileset))
		return false;

	// create generate map, that will generate the noise/number
	generate_map map;
	
	// grab noise
	std::vector<float> level = map.get_map(width, height,tileSize,
		chunkx, chunky, "perlin");

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);
	

	// Attempt to optimize tearing on zooming to a different view.
	//m_tileset.setSmooth(true);
	
	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			// get the current tile number
			int tile_number = get_tile_number(level.at(i + j * width));

			// find its position in the tileset texture
			int tu = tile_number % (m_tileset.getSize().x / (tileSize.x ));
			int tv = tile_number / (m_tileset.getSize().x / (tileSize.x));

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * width) * 4];
			
			// define its 4 corners
			quad[0].position = sf::Vector2f(i * (tileSize.x), j *(tileSize.y));
			quad[1].position = sf::Vector2f((i + 1) * (tileSize.x), j *(tileSize.y));
			quad[2].position = sf::Vector2f((i + 1) * (tileSize.x), (j + 1) *(tileSize.y));
			quad[3].position = sf::Vector2f(i * (tileSize.x), (j + 1) *(tileSize.y));

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * ( tileSize.x ), tv *( tileSize.y ));
			quad[1].texCoords = sf::Vector2f((tu + 1) * ( tileSize.x), tv *( tileSize.y ));
			quad[2].texCoords = sf::Vector2f((tu + 1) * ( tileSize.x), (tv + 1) *( tileSize.y));
			quad[3].texCoords = sf::Vector2f(tu * ( tileSize.x ), (tv + 1) *( tileSize.y));
		}

	return true;
}


void tilemap::set_scale(sf::Vector2f scale)
{
	tilemap::x_scale = scale.x;
	tilemap::y_scale = scale.y;
}
