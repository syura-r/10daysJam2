#include "MapLoader.h"
#include"ObjectManager.h"
#include<fstream>
#include <sstream>
#include "Vector.h"
#include "MapBox.h"
#include "BadGuy.h"
#include "Veranda.h"
#include "Goal.h"
void MapLoader::LoadMap(const std::string& mapName)
{
	std::string filepath = "Resources/Map/" +mapName + ".txt";
	std::ifstream file;
	file.open(filepath.c_str());
	if (!file.is_open())
		return;
	std::string line;
	std::vector<std::vector<int>>map;
	map.resize(29);
	ObjectManager* objectManager = ObjectManager::GetInstance();
	int i = 0;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//カンマ区切りで行の先頭文字を取得
		std::string key;
		while (getline(line_stream, key, ',')) {
			map[i].push_back(atoi(key.c_str()));
		}

		i++;
	}
	file.close();

	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[y].size(); x++)
		{
			Vector3 CreatePos = { -6.0f + x, 7.0f - y,0 };
			switch (map[y][x])
			{
			case 1:
			{
				MapBox* mapBox = new MapBox(CreatePos);
				objectManager->Add(mapBox);
				break;
			}
			case 2:
			{
				BadGuy* badGuy = new BadGuy(CreatePos);
				objectManager->Add(badGuy);
				break;
			}
			case 3:
			{
				MapBox* mapBox = new MapBox(CreatePos);
				objectManager->Add(mapBox);
				break;
			}
			case 4:
			{
				Goal* goal = new Goal(CreatePos);
				objectManager->Add(goal);
				break;
			}
			default:
				break;
			}

		}
	}

}
