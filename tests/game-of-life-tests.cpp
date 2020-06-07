//
// Created by joel on 07/06/2020.
//

#include <catch2/catch.hpp>
#include <game_of_life.hpp>
#include <vector>

TEST_CASE("game assets") {

	using namespace jjo::game_of_life;

	GIVEN("a tie map and a game") {
		sf::RenderWindow window;
		tile_map map(window, 10, 10, 10);
		game game(window, map);

		std::vector<std::string> asset_list{};
		WHEN("no asset has been pushed") {
			THEN("asset list must be empty") {
				asset_list = game.get_asset_names();
				REQUIRE(asset_list.empty());
			}
		}

		WHEN("one asset is pushed") {

			auto asset = std::make_pair(
			    "asset 1",
			    std::vector<std::vector<int>>{
			        {0, 0, 0},
			        {1, 1, 1},
			        {0, 0, 0}});

			REQUIRE_NOTHROW(game.push_asset(asset));

			THEN("asset list must contain one and correct element") {
				asset_list = game.get_asset_names();
				REQUIRE(asset_list.size() == 1);
				REQUIRE(asset_list[0] == "asset 1");
			}
		}

		WHEN("asset is remove from the list") {
			REQUIRE_NOTHROW(game.remove_asset("asset 1"));

			THEN("the list must be empty") {
				asset_list = game.get_asset_names();
				REQUIRE(asset_list.empty());
			}
		}
	}
}
