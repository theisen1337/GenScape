#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "FastNoise.h"
#include "statics_.h"
#include "types.h"

class generate_map
{
public:
	generate_map();
	~generate_map();

	std::vector<float> get_map(const int w, const int h, sf::Vector2u tileSize, int chunkx, int chunky, std::string noise);
private:
	FastNoise noise_gen;
	statics_ s;
};

