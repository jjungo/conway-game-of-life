

include(Catch)

add_executable(game-of-life-tests
		test_main.cpp
		game-of-life-tests.cpp
		)

target_compile_features(game-of-life-tests PRIVATE cxx_std_17)

target_link_libraries(game-of-life-tests
		PRIVATE
		game_of_life
		Catch2::Catch2)

catch_discover_tests(game-of-life-tests)