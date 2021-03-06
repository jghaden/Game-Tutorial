/**
  ******************************************************************************
  * @file    Game.cpp
  * @author  Joshua Haden
  * @version V0.1.0
  * @date    20-MAY-2020
  * @brief   Basic functions in order for the game to operate
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
#include "Player.h"

MAP_TILE MapLegend(int i)
{
	MAP_TILE p;

	switch (i)
	{
		case MAP_BORDER:
			p = { '�', MAP_COLOR_WALL };
			break;
		case MAP_PLAYER:
			p = { '@', MAP_COLOR_PLAYER };
			break;
		case MAP_OBJECTIVE:
			p = { '$', MAP_COLOR_OBJECTIVE };
			break;
		case MAP_DIRT:
			p = { '.', MAP_COLOR_DIRT };
			break;
		case MAP_GRASS:
			p = { '#', MAP_COLOR_GRASS_LIGHT };
			break;
		case MAP_SAND:
			p = { '#', MAP_COLOR_SAND };
			break;
		case MAP_STONE:
			p = { '#', MAP_COLOR_STONE };
			break;
		case MAP_TREE:
			p = { '#', MAP_COLOR_TREE };
			break;
		case MAP_WATER:
			p = { '�', MAP_COLOR_WATER };
			break;
		case MAP_EMPTY:
		default:
			p = { ' ', MAP_COLOR_EMPTY };
			break;
	}

	if(i >= MAP_DOOR_MIN)
		p = { '�', MAP_COLOR_DOOR };

	return p;
}

bool isGameRunning;
bool hasWon;
bool MapChange;
bool isTraveling;
int tmp;

bool isDoor(int i)
{
	return i >= MAP_DOOR_MIN;
}

void Game(std::string fileName)
{
	system("mode 128, 40");
	ShowConsoleCursor(false);

	//SetSize(12, 24);

	//DrawBorder(0, 36, 20, 5);

	Map map(fileName);
	map.Load();
	map.Show();

	Player player(map.playerStart.x, map.playerStart.y);

	isGameRunning = true;
	hasWon = false;
	MapChange = false;
	isTraveling = false;

	while (true)
	{
		if (!(isGameRunning))
		{
			break;
		}

		if (MapChange)
		{
			ClearScreen();
			if (tmp >= MAP_DOOR_MIN)
			{
				map.Load();
				player.x = map.doors[VectorIndex(map.doors, tmp)].pos.x;
				player.y = map.doors[VectorIndex(map.doors, tmp)].pos.y;
			} else
			{
				map.Load();
				player = { map.playerStart.x, map.playerStart.y };
			}
			map.Show();
			MapChange = false;
		}

		player.oX = player.x;
		player.oY = player.y;

		gotoXY(0, map.GetHeight() + 1);
		SetColor(MAP_COLOR_WALL);
		map.Info();
		std::cout << player.x << "x" << player.y << "    " << std::endl;

		SetColor(MAP_COLOR_PLAYER);

		WriteAt(player.x, player.y, MapLegend(MAP_PLAYER).ch);

		if (hasWon)
		{
			break;
		}

		// Player Controls
		switch (_getch())
		{
			case KEY_UP:
				player.y--;
				break;
			case KEY_DOWN:
				player.y++;
				break;
			case KEY_LEFT:
				player.x--;
				break;
			case KEY_RIGHT:
				player.x++;
				break;
			case KEY_RESTART:
				ClearScreen();
				map.Load();
				map.Show();
				player.x = map.playerStart.x;
				player.y = map.playerStart.y;
				break;
			case KEY_ESCAPE:
				isGameRunning = false;
				break;
		}

		tmp = map.GetState(player.x, player.y);

		if (tmp >= MAP_DOOR_MIN && (!isTraveling))
		{
			map.SetFile(map.doors[VectorIndex(map.doors, tmp)].file);
			MapChange = true;
			isTraveling = true;
		} else
		{
			switch (tmp)
			{
				case MAP_GRASS:
					Sleep(15);
					break;
				case MAP_SAND:
					Sleep(60);
					break;
				case MAP_WATER:
					player.x = player.oX;
					player.y = player.oY;
					break;
				case MAP_OBJECTIVE:
					map.objectiveCount--;
					map.SetState(player.x, player.y, MAP_EMPTY);
					if (map.objectiveCount <= 0)
						hasWon = true;
					break;
				case MAP_BORDER:
					player.x = player.oX;
					player.y = player.oY;
					break;
				default:
					if(!(tmp >= MAP_DOOR_MIN))
						isTraveling = false;
					break;
			}
			SetColor(MapLegend(map.GetState(player.oX, player.oY)).color);
			WriteAt(player.oX, player.oY, MapLegend(map.GetState(player.oX, player.oY)).ch);
		}
		if (!isDoor(tmp))
		{
			if (player.y <= 0 && !map.sNORTH.empty())
			{
				map.SetFile(map.sNORTH);
				MapChange = true;
			}
			if (player.y >= map.GetHeight() - 1 && !map.sSOUTH.empty())
			{
				map.SetFile(map.sSOUTH);
				MapChange = true;
			}
			if (player.x <= 0 && !map.sWEST.empty())
			{
				map.SetFile(map.sWEST);
				MapChange = true;
			}
			if (player.x >= map.GetWidth() - 1 && !map.sEAST.empty())
			{
				map.SetFile(map.sEAST);
				MapChange = true;
			}
		}
	}
}