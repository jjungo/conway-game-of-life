enable_testing()
include(Catch)

add_executable(game-of-life-tests
		test_main.cpp
		game-of-life-tests.cpp
		)

target_link_libraries(game-of-life-tests
		PRIVATE
		game-of-life
		Catch2::Catch2)


target_compile_features(game-of-life-tests PRIVATE cxx_std_17)

catch_discover_tests(game-of-life-tests)
