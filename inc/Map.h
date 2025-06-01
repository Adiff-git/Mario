// #include "json.hpp"
// using json = nlohmann::json;


// void Map::LoadFromJsonFile(const std::string& filepath)
// {
// 	clear();
// 	std::ifstream file(filepath);
// 	if (!file) {
// 		std::cerr << "Could not open json file " << filepath << std::endl;
// 		return;
// 	}
// 	nlohmann::json mapJson;
// 	file >> mapJson;

// 	int width = mapJson["width"];
// 	int height = mapJson["height"];
// 	int tilewidth = mapJson["tilewidth"];
// 	std::vector<int> data = mapJson["layers"][0]["data"];

// 	for (int y = 0; y < height; ++y) {
// 		for (int x = 0; x < width; ++x) {
// 			int tileId = data[y * width + x];
// 			std::string str;
// 			switch (tileId) {
// 			case 1:
// 				str = "TILE_B";
// 				break;
// 			case 2:
// 				str = "TILE_E";
// 				break;
// 			case 3:
// 				str = "TILE_F";
// 				break;
// 			case 4:
// 				str = "TILE_A";
// 				break;
// 			case 5:
// 				str = "TILE_D";
// 				break;
// 			case 6:
// 				str = "TILE_H";
// 				break;
// 			case 7:
// 				str = "TILE_Q";
// 				break;
// 			case 8:
// 				str = "TILE_R";
// 				break;
// 			case 9:
// 				str = "TILE_C";
// 				break;
// 			case 10:
// 				str = "TILE_G";
// 				break;
// 			case 11:
// 				str = "TILE_S";
// 				break;
// 			case 12:
// 				str = "TILE_T";
// 				break;
// 			case 13:
// 			case 14:
// 			case 15:
// 			case 16:
// 			case 17:
// 			case 18:
// 				break;
// 			}
// 			if (tileId != 0) {
// 				tiles.push_back(new Tile(Vector2{ (float)x * tilewidth,(float) y * tilewidth },
// 					TileType::TILE_TYPE_NORMAL, "MAP1_" + str));
// 			}
// 		}
// 	}
// }