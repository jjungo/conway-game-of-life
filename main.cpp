#include <SFML/Graphics.hpp>
#include <cstring>

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
		_shape.setOutlineThickness(1);
		_shape.setOutlineColor(sf::Color::Cyan);
	}

public:
	void setColor(const sf::Color &color) {
		_shape.setFillColor(color);
	}

	void draw() {
		_window.draw(_shape);
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

	enum cell_status {
		dead = 0,
		alive
	};

	bool is_alive(int x, int y) const {
		return _current_generation[x + y * _grid_width] == alive;
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

public:
	tile_map(sf::RenderWindow &window, int cell_size, int grid_width, int grid_height)
	    : _window(window)
	    , _cell_size(cell_size)
	    , _grid_width(grid_width)
	    , _grid_height(grid_height) {

		_current_generation = new int[_n_cells];
		memset(_current_generation, 0, _n_cells * sizeof(int));

		_next_generation = new int[_n_cells];
		memset(_next_generation, 0, _n_cells * sizeof(int));
	}

	~tile_map() {
		delete[] _current_generation;
		delete[] _next_generation;
	}

	void on_click(int mx, int my) {
		int x = double(mx) / _cell_size;
		int y = double(my) / _cell_size;
		fprintf(stderr, "%d %d\n", x, y);
		if (x >= 0 && x < _grid_width && y >= 0 && y < _grid_height) {
			_current_generation[x + y * _grid_width] = !_current_generation[x + y * _grid_width];
			fprintf(stderr, "  %d\n", _current_generation[x + y * _grid_width]);
		}
	}

	void generate_next_gen() const {

		for (int x = 0; x < _grid_width; x++) {

			for (int y = 0; y < _grid_height; y++) {

				int n_neighbour_alive = 0;
				int i = -1;
				if (x == 0) {
					i = 0;
				}
				for (; i < 2; i++) {

					int j = -1;
					if (y == 0) {
						j = 0;
					}
					for (; j < 2; j++) {

						if (_current_generation[i * j] == cell_status::alive) {
							n_neighbour_alive++;
						}
					}
				}

				if (n_neighbour_alive < 2) {
					_next_generation[x * y] = cell_status::dead;
				} else if (n_neighbour_alive >= 2 && n_neighbour_alive <= 3) {
					_next_generation[x * y] = cell_status::alive;
				} else {
					_next_generation[x * y] = cell_status::dead;
				}
			}
		}
	}

	void copy_next_to_current_gen() {
		for (int x = 0; x < _grid_width; x++) {
			for (int y = 0; y < _grid_height; y++) {
				_current_generation[x * y] = _next_generation[x * y];
			}
		}
	}

	void draw() {
		generate_and_draw_current_gen();
	}
};

class game {

private:
	sf::RenderWindow &_window;
	tile_map &_map;

	bool _play;
	sf::Clock _clock;
	sf::Text text;
	sf::Font font;

	void draw_texts() {
		if (is_play()) {
			text.setString("running...");
		} else {
			text.setString("paused");
		}
		_window.draw(text);
	}

	void draw_tile_map() {
		_map.draw();
		if (is_play()) {
			if (_clock.getElapsedTime().asMilliseconds() >= 500) {
				_map.generate_next_gen();
				_map.copy_next_to_current_gen();
				_clock.restart();
			}
		}
	}

public:
	game(sf::RenderWindow &window, tile_map &map)
	    : _window(window)
	    , _map(map)
	    , _play(0) {
		font.loadFromFile("../fonts/arial.ttf");
		text.setFont(font);
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::White);
		text.setPosition(10, 0);
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
			}
		}
	}

	void draw() {
		draw_tile_map();
		draw_texts();
	}
};

int main() {
	const int cell_size = 30;
	const int grid_width = 30;
	const int grid_height = 30;

	sf::RenderWindow window(sf::VideoMode(cell_size * grid_width,
	                                      cell_size * grid_height),
	                        "game of life!");
	//	window.setVerticalSyncEnabled(true); // call it once, after creating the window

	tile_map map(window, cell_size, grid_width, grid_height);
	game game(window, map);
	while (window.isOpen()) {

		game.handle_events();

		window.clear();

		game.draw();

		window.display();
	}

	return 0;
}