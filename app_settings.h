#pragma once
#include <string>
#include <vector>
class app_settings
{
public:
	app_settings();
	~app_settings();

	struct key_value {
		std::string name;
		std::string value;
	};

	void load_settings(std::string filepath);

	std::vector<key_value> get_settings();
	std::string get_value(std::string name);

	int get_TILE_W();
	int get_TILE_H();
	int get_CHUNK_H() { return CHUNK_H; }
	int get_CHUNK_W() { return CHUNK_W; }
	int get_WIN_W() { return WIN_W; }
	int get_WIN_H() { return WIN_H; }
	int get_SEED() { return SEED; }

	// IN APP SETTINGS
	int map_scroll_speed = 1;

private:
	// LOAD SETTINGS
	void set_parameters(std::string name, std::string value);

	std::string settings_file = "resources\\settings.config";

	std::vector<std::string> valid_names = { "version", "TILE_H","TILE_W","CHUNK_W","CHUNK_H","WIN_H","WIN_W","SEED"};
	std::vector<key_value> settings;


	std::string version;
	int TILE_H;
	int TILE_W;
	int CHUNK_W;
	int CHUNK_H;
	int WIN_W;
	int WIN_H;
	int SEED;

	
};

