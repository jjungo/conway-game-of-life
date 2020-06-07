#include <SFML/Graphics.hpp>
#include <cstring>
#include <iostream>
#include <random>
#include "game_of_life.hpp"


int main() {

	static constexpr int cell_size = 10;
	static constexpr int grid_width = 100;
	static constexpr int grid_height = 100;
	static constexpr int win_width = cell_size * grid_width;
	static constexpr int win_height = cell_size * grid_height;

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Conway's Game of Life");
	window.setVerticalSyncEnabled(true);// call it once, after creating the window

	jjo::game_of_life::tile_map map(window, cell_size, grid_width, grid_height);
	jjo::game_of_life::game game(window, map);
	while (window.isOpen()) {
		game.handle_events();
		window.clear();
		game.draw();
		window.display();
	}
	return 0;
}