//
// Created by joel on 07/06/2020.
//

#ifndef GAME_OF_LIFE__GAME_OF_LIFE_HPP
#define GAME_OF_LIFE__GAME_OF_LIFE_HPP
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstring>
#include <iostream>
#include <random>
#include <thread>
#include <unordered_map>

namespace jjo::game_of_life {

class simple_rand_generator {
private:
	std::random_device _rd;
	std::mt19937 _gen;
	std::uniform_int_distribution<> _distrib;

public:
	simple_rand_generator(int min, int max)
	    : _gen(_rd())
	    , _distrib(min, max) {}

	int get_next_rand() {
		return _distrib(_gen);
	}
};

class benchmark {
private:
	std::chrono::steady_clock::time_point _start;
	sf::Clock _bench_timer;
	const sf::Clock _game_clock;
	float _frame_rate;
	float _last_timestamp;

	bool _started{false};

	long get_elapsed_time() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start).count();
	}

	static void show(int n_cells, long elapsed_ms) {
		fprintf(stderr, "%f,\n", (float) n_cells / ((float) elapsed_ms / 1000.f));
	}

	void show_cell_stat(int cell_count) {
		static int cells = 0;
		cells += cell_count;
		if (_bench_timer.getElapsedTime().asSeconds() > 1) {

			if (!_started) {
				return;
			}

			long elapsed_ms = get_elapsed_time();
			show(cells, elapsed_ms);
			start();

			_bench_timer.restart();
			cells = 0;
		}
	}

public:
	void start() {
		_start = std::chrono::steady_clock::now();
		_started = true;
	}

	void run(int cell_count) {
		show_cell_stat(cell_count);
		update_frame_rate();
	}

	void update_frame_rate() {
		auto now = _game_clock.getElapsedTime().asSeconds();
		_frame_rate = 1.f / (now - _last_timestamp);
		_last_timestamp = now;
	}

	float get_frame_rate() const {
		return _frame_rate;
	}
};

class tile_map {

private:
	sf::RenderWindow &_window;
	int _cell_size;
	int _grid_width;
	int _grid_height;
	int _n_cells;

	int *_current_generation;
	int *_next_generation;

	simple_rand_generator _rand_gen;
	sf::Vertex *_cells;

	enum cell_status {
		dead = 0,
		alive
	};

	sf::Color alive_color = sf::Color::Yellow;
	sf::Color dead_color = sf::Color::Black;

public:
	tile_map(sf::RenderWindow &window, int cell_size, int grid_width, int grid_height)
	    : _window(window)
	    , _cell_size(cell_size)
	    , _grid_width(grid_width)
	    , _grid_height(grid_height)
	    , _n_cells(_grid_height * _grid_width)
	    , _current_generation(new int[_n_cells])
	    , _next_generation(new int[_n_cells])
	    , _rand_gen(0, _n_cells)
	    , _cells(new sf::Vertex[_n_cells]) {

		clear();
		shuffle_grid();
		//		create_cells();
	}

	~tile_map() {
		delete[] _current_generation;
		delete[] _next_generation;
		delete[] _cells;
	}

	int cell_count() const {
		return _n_cells;
	}

	void on_click(int mx, int my) {
		int x = mx / _cell_size;
		int y = my / _cell_size;

		fprintf(stderr, "[+]%d %d [%d]\n", x, y, x + y * _grid_width);

		if (x >= 0 && x < _grid_width && y >= 0 && y < _grid_height) {
			fprintf(stderr, "%d %d [%d]\n", x, y, x + y * _grid_width);
			_current_generation[x + y * _grid_width] = !_current_generation[x + y * _grid_width];
			update_cell(x, y);
		}
	}

	void generate_and_draw_current_gen() {
		//		for (int x = 0; x < _grid_width; x++) {
		//			for (int y = 0; y < _grid_height; y++) {
		//
		//				//				auto cell = _cells[x + y * _grid_width];
		//				auto color = (is_alive(x, y)
		//				                  ? sf::Color::Yellow
		//				                  : sf::Color::Black);
		//				//				cell->draw();
		//
		//				_cells.setPixel(x, y, color);
		//			}
		//		}

		//		_texture.update(_cells);
		//		_window.draw(_sprite);

		_window.draw(_cells, _n_cells, sf::Points);
	}

	void generate_random_cell() {
		auto n = _rand_gen.get_next_rand();
		_current_generation[n] = cell_status::alive;
	}

	void generate_next_gen() {

		for (int x = 0; x < _grid_width; x++) {
			int x0 = ((x + _grid_width - 1) % _grid_width);
			int x1 = ((x + _grid_width) % _grid_width);
			int x2 = ((x + _grid_width + 1) % _grid_width);

			for (int y = 0; y < _grid_height; y++) {
				int y0 = ((y + _grid_width - 1) % _grid_width) * _grid_width;
				int y1 = ((y + _grid_width) % _grid_width) * _grid_width;
				int y2 = ((y + _grid_width + 1) % _grid_width) * _grid_width;

				int neighbours = 0;
				neighbours = _current_generation[x0 + y0] + _current_generation[x1 + y0] + _current_generation[x2 + y0]
				    + _current_generation[x0 + y1] + _current_generation[x2 + y1]
				    + _current_generation[x0 + y2] + _current_generation[x1 + y2] + _current_generation[x2 + y2];

				generate_next_cell(x, y, neighbours);
			}
		}
	}

	inline void copy_next_to_current_gen() {
		std::swap(_current_generation, _next_generation);
	}

	void drop_asset(const std::vector<std::vector<int>> &asset, int x, int y) {
		if (asset.empty()) {
			return;
		}

		// TODO check window boundaries
		if (x < 0 || y < 0) {
			return;
		}

		// TODO fix this indexing mess, it's onl work when asset dimension is N x N
		for (unsigned int i = 0; i < asset.size(); i++) {
			for (unsigned int j = 0; j < asset.at(i).size(); j++) {
				_current_generation[(x + i) + (y + j) * _grid_width] = asset[j][i];
			}
		}
	}

	void clear() {
		memset(_current_generation, 0, _n_cells * sizeof(*_current_generation));
		memset(_next_generation, 0, _n_cells * sizeof(*_next_generation));
	}

	void shuffle_grid() const {
		simple_rand_generator rnd(0, 1);
		for (int i = 0; i < _n_cells; i++) {
			_current_generation[i] = rnd.get_next_rand();
			_next_generation[i] = rnd.get_next_rand();
		}
	}

private:
	inline bool is_alive(int x, int y) const {
		return _current_generation[x + y * _grid_width] == alive;
	}

	inline void generate_next_cell(int x, int y, int &neib) const {

		int index = x + y * _grid_width;

		_next_generation[index] = _current_generation[index];
		if (_current_generation[index] == alive && (neib < 2 || neib > 3)) {
			_next_generation[index] = dead;
		} else if (neib == 3) {
			_next_generation[index] = alive;
		}

		update_cell(x, y);
	}

	void update_cell(int x, int y) const {
		int index = x + y * _grid_width;
		_cells[index].position = sf::Vector2f(x, y);
		_cells[index].color = (_next_generation[index] == alive) ? alive_color : dead_color;
	}

	//	void create_cells() {
	//		for (int x = 0; x < _grid_width; x++) {
	//			for (int y = 0; y < _grid_width; y++) {
	//				_cells.insert({x + y * _grid_width, std::make_shared<cell>(_window, x, y, _cell_size)});
	//			}
	//		}
	//	}
};

class game {

private:
	sf::RenderWindow &_window;
	tile_map &_map;

	bool _play;
	bool _verbose;
	bool _shuffle_grid;
	sf::Clock _refresh_timer;
	sf::Clock _random_cell_timer;
	sf::Text _is_playing_text;
	sf::Text _frame_rate_text;
	sf::Font _font;

	int _refresh_period_ms;

	using asset_name = std::string;
	using asset = std::vector<std::vector<int>>;
	std::unordered_map<asset_name, asset> _assets;

	benchmark _bench;

	void draw_texts() {
		std::string str = (is_play()) ? "running..." : "paused";
		_is_playing_text.setString(str);
		_window.draw(_is_playing_text);

		std::string frame_rate_str = std::to_string(_bench.get_frame_rate());
		_frame_rate_text.setString(frame_rate_str);
		_window.draw(_frame_rate_text);
	}

	void draw_tile_map() {
		_map.generate_and_draw_current_gen();

		if (is_play()) {

			//			if (_random_cell_timer.getElapsedTime().asMilliseconds() > 10.0) {
			//				_map.generate_random_cell();
			//				_random_cell_timer.restart();
			//			}

			//			if (_refresh_timer.getElapsedTime().asMilliseconds() >= _refresh_period_ms) {
			_map.generate_next_gen();
			_map.copy_next_to_current_gen();
			_refresh_timer.restart();
			//			}
		}

		_bench.run(_map.cell_count());
	}

	void setup_text() {
		_font.loadFromFile("../fonts/arial.ttf");
		_is_playing_text.setFont(_font);
		_is_playing_text.setCharacterSize(30);
		_is_playing_text.setFillColor(sf::Color::Cyan);
		_is_playing_text.setPosition(10, 0);

		_frame_rate_text.setFont(_font);
		_frame_rate_text.setCharacterSize(30);
		_frame_rate_text.setFillColor(sf::Color::Cyan);
		_frame_rate_text.setPosition(10, 60);
	};

public:
	game(sf::RenderWindow &window, tile_map &map,
	     int refresh_period_ms = 16,
	     bool enable_bench = false,
	     bool autoplay = false)
	    : _window(window)
	    , _map(map)
	    , _play(autoplay)
	    , _verbose(true)
	    , _shuffle_grid(false)
	    , _refresh_period_ms(refresh_period_ms) {

		setup_text();

		if (enable_bench) {
			_bench.start();
		}

		fprintf(stdout, "Keymaps: \n");
		fprintf(stdout, " [+] c: clear \n");
		fprintf(stdout, " [+] p: pause / resume\n\n");
	}

	bool is_play() const {
		return _play;
	}

	inline bool is_play() {
		return _play;
	}

	void handle_events() {
		sf::Event event{};
		while (_window.pollEvent(event)) {

			switch (event.type) {
			case sf::Event::Closed:
				_window.close();
				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left) {
					_map.on_click(event.mouseButton.x, event.mouseButton.y);
				}
				break;

			case sf::Event::KeyPressed: {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
					_play = !_play;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
					_map.clear();
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
					_verbose = !_verbose;
				} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					_shuffle_grid = !_shuffle_grid;
				}
				break;
			}

			default:
				break;
			}
		}
	}

	void draw() {
		draw_tile_map();

		if (_verbose) {
			draw_texts();
		}

		if (_shuffle_grid) {
			_shuffle_grid = false;
			_map.shuffle_grid();
		}
	}

	void push_asset(const std::pair<asset_name, asset> &asset) {
		_assets.insert(asset);
	}

	void remove_asset(const std::string &asset_name) {
		_assets.erase(asset_name);
	}

	std::vector<std::string> get_asset_names() const {
		std::vector<std::string> asset_list;
		for (const auto &[name, _] : _assets) {
			(void) _;// unused
			asset_list.push_back(name);
		}

		return asset_list;
	}

	void drop_asset(const asset_name &name, int x, int y) {
		auto asset = _assets[name];
		_map.drop_asset(asset, x, y);
	}
};// namespace jjo::game_of_life

}// namespace jjo::game_of_life
#endif//GAME_OF_LIFE__GAME_OF_LIFE_HPP
