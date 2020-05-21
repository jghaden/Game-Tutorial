/**
  ******************************************************************************
  * @file    MapHandler.h
  * @author  Joshua Haden
  * @version V0.1.0
  * @date    20-MAY-2020
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
	Door(int id, _COORDS pos, std::string file);
};

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

	Map(std::string file);

	// Getters
	std::string GetFile();

	int GetState(int x, int y);
	int GetWidth();
	int GetHeight();

	// Setters
	void SetFile(std::string file);
	void SetState(int x, int y, int i);

	// Main
	void Info();
	void Init();
	void Load();
	void Show();
};

int VectorIndex(std::vector<Door> &list, int n);