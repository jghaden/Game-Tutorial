/**
  ******************************************************************************
  * @file    MapHandler.h
  * @author  Joshua Haden
  * @version V0.0.0
  * @date    30-DEC-2019
  * @brief   Header for MapHandler.cpp
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#include "PCH.h"

struct _COORDS
{
	int x, y;
};

struct Door
{
public:
	int id;
	_COORDS pos;
	std::string file;

	Door() {}
	Door(int id, _COORDS pos, std::string file)
	{
		this->id = id;
		this->pos = pos;
		this->file = file;
	}
};

int VectorIndex(std::vector<Door> &list, int n);

class Map
{
private:
	int id, width, height;
	std::string name, file;
	std::vector<std::vector<int>> grid;
public:
	std::string sNORTH, sSOUTH, sWEST, sEAST;
	std::vector<Door> doors;
	_COORDS playerStart = { -1,-1 };
	int objectiveCount;

	Map(std::string file)
	{
		this->file = file;
	}

	void info()
	{
		std::cout << "Map ID:\t\t" << id << std::endl;
		std::cout << "Map Name:\t" << name << std::endl;
		std::cout << "Size:\t\t" << width << 'x' << height << std::endl;
		std::cout << "Objective:\t" << objectiveCount << std::endl;
	}

	void init()
	{
		grid.clear();
		doors.clear();
		grid.resize(height, std::vector<int>(width));
	}

	void setFile(std::string file)
	{
		this->file = file;
	}

	void setIntAt(int x, int y, int i)
	{
		grid[y][x] = i;
	}

	std::string GetFile()
	{
		return file;
	}

	int getIntAt(int x, int y)
	{
		if (x < 0 || y < 0 || x > width - 1 || y > height - 1)
			return 1;
		else
			return grid[y][x];
	}

	int GetWidth()
	{
		return width;
	}

	int GetHeight()
	{
		return height;
	}

	void Load();
	void Show();
};