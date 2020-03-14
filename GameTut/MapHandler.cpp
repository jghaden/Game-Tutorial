#include "pch.h"
#include "Game.h"
#include "MapHandler.h"

int VectorIndex(std::vector<Door> &list, int n)
{
	int tmp = -1;

	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].id == n)
		{
			tmp = i;
			break;
		}
	}

	return tmp;
}

void Map::Load()
{
	int tmp;
	std::ifstream inFile;
	std::string tmpS;

	inFile.open("res/rooms/" + file);
	if (inFile.is_open())
	{
		inFile >> std::hex >> id;
		inFile.ignore(1, '\n');

		getline(inFile, name);

		inFile >> std::dec >> width;
		inFile >> std::dec >> height;

		init();

		inFile.ignore(1, '\n');

		objectiveCount = 0;

		_COORDS* loc = new _COORDS;
		Door* door = new Door;

		for (int y = 0; y < height; y++)
		{			
			for (int x = 0; x < width; x++)
			{
				inFile >> tmp;
				inFile.ignore(1, ',');

				if (tmp >= MAP_DOOR_MIN)
				{
					loc->x = x;
					loc->y = y;

					door->id = tmp;
					door->pos = { loc->x, loc->y };
					door->file = "EMPTY";

					doors.push_back(*door);

					grid[y][x] = tmp;
				} else
				{
					switch (tmp)
					{
						case MAP_PLAYER:
							playerStart = { x,y };
							break;
						case MAP_OBJECTIVE:
							grid[y][x] = MAP_OBJECTIVE;
							objectiveCount++;
							break;
						default:
							grid[y][x] = tmp;
							break;
					}
				}
			}
			inFile.ignore(1, '\n');
		}

		if (playerStart.x == -1 || playerStart.y == -1)
		{
			inFile >> playerStart.x >> playerStart.y;
		}

		while (!inFile.eof())
		{
			inFile >> tmp >> tmpS;

			// Doors
			if (tmp >= MAP_DOOR_MIN)
			{
				doors[VectorIndex(doors, tmp)].file = tmpS;
			} else
			{
				switch (tmp)
				{
					case MAP_NORTH:
						sNORTH = tmpS;
						break;
					case MAP_SOUTH:
						sSOUTH = tmpS;
						break;
					case MAP_WEST:
						sWEST = tmpS;
						break;
					case MAP_EAST:
						sEAST = tmpS;
						break;
				}
			}
		}

		delete loc;
		delete door;

		inFile.close();
	}
	else
	{
		std::cout << "MAP_HANDLER_ERROR: Could not load file" << std::endl;
	}
}

void Map::Show()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			SetColor(MapLegend(getIntAt(x, y)).color);
			WriteAt(x, y, MapLegend(getIntAt(x, y)).ch);
		}
		std::cout << std::endl;
	}
}