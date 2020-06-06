cmake_minimum_required(VERSION 3.16)
project(game-of-life)


set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
			STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
			"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()


add_executable(game-of-life main.cpp)

target_link_libraries(
		game-of-life
		PRIVATE
		sfml-graphics
		sfml-window
		sfml-system
)
target_compile_options(game-of-life PRIVATE -Wall -Wextra)
target_compile_features(game-of-life PRIVATE cxx_std_14)