/**
  ******************************************************************************
  * @file    Game.h
  * @author  Joshua Haden
  * @version V0.1.0
  * @date    20-MAY-2020
  * @brief   Header for Game.cpp
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#include "PCH.h"

// Controls
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESCAPE 27
#define KEY_RESTART 114

// Map legend values
#define MAP_EMPTY 0
#define MAP_BORDER 1
#define MAP_PLAYER 2
#define MAP_OBJECTIVE 3

#define MAP_DIRT 4
#define MAP_GRASS 5
#define MAP_SAND 6
#define MAP_STONE 7
#define MAP_TREE 8

#define MAP_WATER 9

// Map markers
#define MAP_NORTH 100
#define MAP_SOUTH 101
#define MAP_WEST 103
#define MAP_EAST 104

#define MAP_DOOR_MIN 512

// Map colors
#define MAP_COLOR_EMPTY 0x00
#define MAP_COLOR_WALL 0x08
#define MAP_COLOR_PLAYER 0x0D
#define MAP_COLOR_OBJECTIVE 0x0A

#define MAP_COLOR_DIRT 0xE
#define MAP_COLOR_GRASS_DARK 0x2
#define MAP_COLOR_GRASS_LIGHT 0xA
#define MAP_COLOR_SAND 0xE
#define MAP_COLOR_STONE 0x8
#define MAP_COLOR_TREE 0x2

#define MAP_COLOR_WATER 0x9
#define MAP_COLOR_DOOR 0xC

#define BORDER_TYPE_MAP 0

struct MAP_TILE
{
	char ch;
	int color;
};

MAP_TILE MapLegend(int i);

bool isDoor(int i);

void Game(std::string fileName);
