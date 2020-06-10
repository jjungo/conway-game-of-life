
#include "game_of_life.hpp"

static void push_assets(jjo::game_of_life::game &game) {
	auto blinker = std::make_pair(
	    "Blinker",
	    std::vector<std::vector<int>>{
	        {0, 1, 0},
	        {0, 1, 0},
	        {0, 1, 0}});

	game.push_asset(blinker);
	game.drop_asset(blinker.first, 10, 10);

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
	game.drop_asset(toad.first, 20, 20);

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
	game.drop_asset(glider.first, 30, 10);

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
	game.drop_asset(hwss.first, 50, 50);

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
	game.drop_asset(loafer.first, 500, 500);
}
int app() {

	static constexpr int refresh_period_ms = 1;
	static constexpr int cell_size = 1;
	static constexpr int grid_width = 2050;
	static constexpr int grid_height = 1100;
	static constexpr int win_width = cell_size * grid_width;
	static constexpr int win_height = cell_size * grid_height;
	static constexpr bool enable_benchmark = true;
	static constexpr bool autoplay = true;

	sf::RenderWindow window(sf::VideoMode(grid_width, grid_height), "Conway's Game of Life");
	window.setVerticalSyncEnabled(true);// call it once, after creating the window

	jjo::game_of_life::tile_map map(window, cell_size, grid_width, grid_height);
	jjo::game_of_life::game game(window, map, refresh_period_ms, autoplay, enable_benchmark);

	//		push_assets(game);

	while (window.isOpen()) {
		game.handle_events();
		window.clear();
		game.draw();
		window.display();
	}
	return 0;
}

int main() {

	app();

	return 0;
	sf::RenderWindow window(sf::VideoMode(100, 100), "Conway's Game of Life");
	window.setVerticalSyncEnabled(true);// call it once, after creating the window

	sf::Image _cells;
	sf::Texture texture;
	if (!texture.create(100, 100))
		return -1;

	_cells.create(100, 100);
	// Create a sprite that will display the texture
	sf::Sprite sprite;
	sprite.setTexture(texture);
	while (1)// the main loop
	{

		window.clear();

		// update the texture
		_cells.setPixel(50, 50, sf::Color::Red);
		texture.update(_cells);
		// draw it
		window.draw(sprite);

		window.display();
	}
}