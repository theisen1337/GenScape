#include "app_settings.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>


app_settings::app_settings()
{
	load_settings("resources\\settings.config");
}


app_settings::~app_settings()
{
}


void app_settings::load_settings(std::string filepath)
{
	std::string line;
	std::ifstream infile(filepath);
	if (infile.is_open())
	{
		std::cout << "opened " << filepath << std::endl;
		while (std::getline(infile, line))
		{

			std::cout << "loading: "<< line;

			std::string value = line.substr(line.find_first_of('=') + 1, line.length());
			std::string name = line.substr(0, line.find_first_of('='));
			set_parameters(name, value);
			std::cout << "  ..success!" << std::endl;

		}
		//closing file
		infile.close();

	}
	else
	{
		std::cout << "error opening " << filepath << std::endl;
		//closing file
		infile.close();
	}
	
	app_settings::TILE_H = std::stoi(app_settings::get_value("TILE_H"));
	app_settings::TILE_W = std::stoi(app_settings::get_value("TILE_W"));
	app_settings::CHUNK_H = std::stoi(app_settings::get_value("CHUNK_H"));
	app_settings::CHUNK_W = std::stoi(app_settings::get_value("CHUNK_W"));
	app_settings::WIN_H = std::stoi(app_settings::get_value("WIN_H"));
	app_settings::WIN_W = std::stoi(app_settings::get_value("WIN_W"));
	app_settings::SEED = std::stoi(app_settings::get_value("SEED"));
}


std::vector<app_settings::key_value> app_settings::get_settings()
{
	return settings;
}

std::string app_settings::get_value(std::string name)
{
	for (int i = 0; i < settings.size(); i++)
		if (name == settings.at(i).name)
			return settings.at(i).value;

	return "NULL";
}

int app_settings::get_TILE_W()
{
	return TILE_W;
}

int app_settings::get_TILE_H()
{
	return TILE_H;
}

void app_settings::set_parameters(std::string name, std::string value)
{
	for (int i = 0; i < valid_names.size(); i++)
	{
		if (name == valid_names.at(i))
		{
			settings.push_back({ name, value });
			std::cout << "";
			return;
		}
	}
	std::cout << "Error: " << name << "=" << value << " is an incorrect setting." << std::endl;
	exit(1);
	
}
