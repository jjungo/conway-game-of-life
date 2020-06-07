//
// Created by joel on 07/06/2020.
//

#ifndef GAME_OF_LIFE__GAME_OF_LIFE_HPP
#define GAME_OF_LIFE__GAME_OF_LIFE_HPP
#include <SFML/Graphics.hpp>
#include <cstring>
#include <iostream>
#include <random>

namespace jjo::game_of_life {

class cell {
private:
	sf::RenderWindow &_window;
	int _pos_x;
	int _pos_y;
	int _size;

	sf::RectangleShape _shape;

public:
	cell(sf::RenderWindow &window, int x, int y, int size)
	    : _window(window)
	    , _pos_x(x)
	    , _pos_y(y)
	    , _size(size)
	    , _shape(sf::Vector2f(_size, _size)) {

		_shape.setPosition(_pos_x * _size, _pos_y * _size);
		//		_shape.setOutlineThickness(0.8);
		//		_shape.setOutlineColor(sf::Color::Green);
	}

public:
	void setColor(const sf::Color &color) {
		_shape.setFillColor(color);
	}

	void draw() {
		_window.draw(_shape);
	}
};

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

class tile_map {

private:
	sf::RenderWindow &_window;
	int _cell_size;
	int _grid_width;
	int _grid_height;

	const int _n_cells = _grid_height * _grid_width;
	int *_current_generation;
	int *_next_generation;

	simple_rand_generator _rand_gen;

	enum cell_status {
		dead = 0,
		alive
	};

	bool is_alive(int x, int y) const {
		return _current_generation[x + y * _grid_width] == alive;
	}

	void generate_next_cell(int x, int y, int i, int j, int &neib) const {

		if (_current_generation[(x + i) + (y + j) * _grid_width] == alive) {
			neib++;
		}

		int index = x + y * _grid_width;

		_next_generation[index] = _current_generation[index];
		if (_current_generation[index] == alive && (neib < 2 || neib > 3)) {
			_next_generation[index] = dead;
		} else if (neib == 3) {
			_next_generation[index] = alive;
		}
	}

public:
	tile_map(sf::RenderWindow &window, int cell_size, int grid_width, int grid_height)
	    : _window(window)
	    , _cell_size(cell_size)
	    , _grid_width(grid_width)
	    , _grid_height(grid_height)
	    , _rand_gen(0, _n_cells) {

		_current_generation = new int[_n_cells];
		memset(_current_generation, 0, _n_cells * sizeof(*_current_generation));

		_next_generation = new int[_n_cells];
		memset(_next_generation, 0, _n_cells * sizeof(*_next_generation));
	}

	~tile_map() {
		delete[] _current_generation;
		delete[] _next_generation;
	}

	void on_click(int mx, int my) {
		int x = mx / _cell_size;
		int y = my / _cell_size;
		if (x >= 0 && x < _grid_width && y >= 0 && y < _grid_height) {
			_current_generation[x + y * _grid_width] = !_current_generation[x + y * _grid_width];
		}
	}

	void generate_and_draw_current_gen() const {
		for (int x = 0; x < _grid_width; x++) {
			for (int y = 0; y < _grid_height; y++) {
				cell cell(_window, x, y, _cell_size);
				cell.setColor((is_alive(x, y))
				                  ? sf::Color::Yellow
				                  : sf::Color::Black);
				cell.draw();
			}
		}
	}

	void enable_random_cell() {
		auto n = _rand_gen.get_next_rand();
		fprintf(stderr, "n: %d\n", n);
		_current_generation[n] = cell_status::alive;
	}

	void generate_next_gen() const {

		for (int x = 0; x < _grid_width; x++) {
			for (int y = 0; y < _grid_height; y++) {
				int neib = 0;
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {

						if (j == 0 && i == 0)
							continue;

						if ((x + i) == _grid_width - 1)
							continue;

						if ((y + j) == _grid_height - 1)
							continue;

						generate_next_cell(x, y, i, j, neib);
					}
				}
			}
		}
	}

	void copy_next_to_current_gen() {
		for (int x = 0; x < _grid_width; x++) {
			for (int y = 0; y < _grid_height; y++) {
				_current_generation[x + y * _grid_width] = _next_generation[x + y * _grid_width];
			}
		}
	}
};

class game {

private:
	sf::RenderWindow &_window;
	tile_map &_map;

	bool _play;
	sf::Clock _refresh_timer;
	sf::Clock _random_cell_timer;
	sf::Text _text;
	sf::Font _font;

	void draw_texts() {
		std::string str = (is_play()) ? "running..." : "paused";
		_text.setString(str);
		_window.draw(_text);
	}

	void draw_tile_map() {
		_map.generate_and_draw_current_gen();
		if (is_play()) {

			if (_random_cell_timer.getElapsedTime().asMilliseconds() > 10.0) {
				//				_map.enable_random_cell();
				//				_random_cell_timer.restart();
			}

			if (_refresh_timer.getElapsedTime().asMilliseconds() >= 66) {
				_map.generate_next_gen();
				_map.copy_next_to_current_gen();
				_refresh_timer.restart();
			}
		}
	}

public:
	game(sf::RenderWindow &window, tile_map &map)
	    : _window(window)
	    , _map(map)
	    , _play(false) {
		_font.loadFromFile("../fonts/arial.ttf");
		_text.setFont(_font);
		_text.setCharacterSize(15);
		_text.setFillColor(sf::Color::White);
		_text.setPosition(10, 0);
	}

	bool is_play() const {
		return _play;
	}

	bool is_play() {
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
		draw_texts();
	}
};

}// namespace jjo::game_of_life
#endif//GAME_OF_LIFE__GAME_OF_LIFE_HPP
