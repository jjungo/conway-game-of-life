cmake_minimum_required(VERSION 3.16)
project(game-of-life LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(utils)

find_catch2()

set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
			STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
			"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()


add_library(game-of-life INTERFACE)
target_compile_features(game-of-life INTERFACE cxx_std_17)

target_include_directories(
		game-of-life
		INTERFACE
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>
)
target_link_libraries(game-of-life
		INTERFACE
		sfml-graphics
		sfml-window
		sfml-system
		)

target_compile_options(game-of-life INTERFACE -Wall -Wextra)
target_compile_features(game-of-life INTERFACE cxx_std_17)


add_executable(game-of-life-app
		main.cpp)

target_link_libraries(game-of-life-app
		PRIVATE
		game-of-life
		)
target_compile_options(game-of-life-app PRIVATE -Wall -Wextra)
target_compile_features(game-of-life-app PRIVATE cxx_std_17)

add_subdirectory(tests)