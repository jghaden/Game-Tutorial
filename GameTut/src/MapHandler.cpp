/**
  ******************************************************************************
  * @file    MapHandler.cpp
  * @author  Joshua Haden
  * @version V0.1.0
  * @date    20-MAY-2020
  * @brief   Load and display map from file to console
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#include "PCH.h"

#include "System.h"
#include "Game.h"
#include "MapHandler.h"

Door::Door(int id, _COORDS pos, std::string file)
{
	this->id = id;
	this->pos = pos;
	this->file = file;
}

Map::Map(std::string file)
{
	this->file = file;
}

// Getters
int Map::GetWidth() { return width; }
int Map::GetHeight() { return height; }

std::string Map::GetFile()
{
	return file;
}

int Map::GetState(int x, int y)
{
	if (x < 0 || y < 0 || x > width - 1 || y > height - 1)
		return 1;
	else
		return grid[y][x];
}

// Setters
void Map::SetFile(std::string file) { this->file = file; }
void Map::SetState(int x, int y, int i) { grid[y][x] = i; }

// Main
void Map::Info()
{
	std::cout << "Map ID:\t\t" << id << std::endl;
	std::cout << "Map Name:\t" << name << std::endl;
	std::cout << "Size:\t\t" << width << 'x' << height << std::endl;
	std::cout << "Objective:\t" << objectiveCount << std::endl;
}

void Map::Init()
{
	grid.clear();
	doors.clear();
	grid.resize(height, std::vector<int>(width));
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

		Init();

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
			SetColor(MapLegend(GetState(x, y)).color);
			WriteAt(x, y, MapLegend(GetState(x, y)).ch);
		}
		std::cout << std::endl;
	}
}

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