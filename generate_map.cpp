#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include "generate_map.h"
#include "FastNoise.h"


generate_map::generate_map()
{
	
	srand(s.get_settings().get_SEED());
	noise_gen.SetFractalType(FastNoise::FBM);
	noise_gen.SetSeed(s.get_settings().get_SEED());
	noise_gen.SetFractalGain(0.6);
	noise_gen.SetFrequency(0.01);
	noise_gen.SetFractalOctaves(5);
}


generate_map::~generate_map()
{
}


std::vector<float> generate_map::get_map(int w, int h, sf::Vector2u tileSize, int offsetx, int offsety, std::string noise)
{
	std::vector<float> level;
	if (noise == "perlin")
	{
		/*

			Perlin noise, position noise.

		*/

		//update width and height
		int x_i = offsetx / (int)tileSize.x;
		int y_i = offsety / (int)tileSize.y;

		w = w + x_i;
		h = h + y_i;

		for (int y = y_i; y < h; y++)
		{
			for (int x = x_i; x < w; x++)
			{
				level.push_back(noise_gen.GetNoise(x, y));
			}
		}
		return level;

	}
	else
	{
		for (int i = 0; i < w * h; i++)
			level.push_back(rand() % 4);

		return level;
	}
}
