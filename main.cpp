
#include "game_of_life.hpp"

static void push_assets(jjo::game_of_life::game &game) {
	auto blinker = std::make_pair(
	    "Blinker",
	    std::vector<std::vector<int>>{
	        {0, 1, 0},
	        {0, 1, 0},
	        {0, 1, 0}});

	game.push_asset(blinker);
	game.place_asset(blinker.first, 10, 10);

	auto toad = std::make_pair(
	    "toad",
	    std::vector<std::vector<int>>{
	        {0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 0, 0, 0},
	        {0, 0, 1, 1, 1, 0},
	        {0, 1, 1, 1, 0, 0},
	        {0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 0, 0, 0},
	    });

	game.push_asset(toad);
	game.place_asset(toad.first, 20, 20);

	auto glider = std::make_pair(
	    "Glider",
	    std::vector<std::vector<int>>{
	        {0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 1, 0, 0},
	        {0, 1, 0, 1, 0, 0},
	        {0, 0, 1, 1, 0, 0},
	        {0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 0, 0, 0},
	    });

	game.push_asset(glider);
	game.place_asset(glider.first, 30, 10);

	auto hwss = std::make_pair(
	    "HWSS",// Heavy-weight	spaceship
	    std::vector<std::vector<int>>{
	        {0, 0, 0, 0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 0, 1, 1, 0, 0, 0},
	        {0, 0, 1, 0, 0, 0, 0, 1, 0},
	        {0, 1, 0, 0, 0, 0, 0, 0, 0},
	        {0, 1, 0, 0, 0, 0, 0, 1, 0},
	        {0, 1, 1, 1, 1, 1, 1, 0, 0},
	        {0, 0, 0, 0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 0, 0, 0, 0, 0, 0},
	    });

	game.push_asset(hwss);
	game.place_asset(hwss.first, 50, 50);

	auto loafer = std::make_pair(
	    "loafer",
	    std::vector<std::vector<int>>{
	        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	        {0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0},
	        {0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0},
	        {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
	        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	        {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
	        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
	        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    });

	game.push_asset(loafer);
	game.place_asset(loafer.first, 70, 60);
}
int main() {

	static constexpr int refresh_period_ms = 32;
	static constexpr int cell_size = 10;
	static constexpr int grid_width = 100;
	static constexpr int grid_height = 100;
	static constexpr int win_width = cell_size * grid_width;
	static constexpr int win_height = cell_size * grid_height;

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Conway's Game of Life");
	window.setVerticalSyncEnabled(true);// call it once, after creating the window

	jjo::game_of_life::tile_map map(window, cell_size, grid_width, grid_height);
	jjo::game_of_life::game game(window, map, refresh_period_ms);

	push_assets(game);

	while (window.isOpen()) {
		game.handle_events();
		window.clear();
		game.draw();
		window.display();
	}
	return 0;
}