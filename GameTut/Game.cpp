#include "PCH.h"
#include "Game.h"
#include "MapHandler.h"
#include "Player.h"

void gotoXY(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

template<typename T>
void WriteAt(int x, int y, T data)
{
	gotoXY(x, y);
	std::cout << data;
}

void DrawBorder(int x, int y, int w, int h)
{
	gotoXY(x, y);
	SetColor(0xf);
	
	WriteAt(x, y, 'Ú');
	WriteAt(x + w, y, '¿');

	WriteAt(x, y + h, 'À');
	WriteAt(x, y + h, 'Ù');

	for (int i = x; i < w; i++)
	{
		WriteAt(i, y, 'Ä');
		WriteAt(i, h - 1, 'Ä');
	}

	for (int j = y; j < h; j++)
	{
		WriteAt(x, j, '³');
		WriteAt(w, j, '³');
	}
}

void SetColor(int c)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, c);
}

void SetSize(int w, int h)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = w;
	cfi.dwFontSize.Y = h;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

PIXEL MapLegend(int i)
{
	PIXEL p;

	switch (i)
	{
		case MAP_BORDER:
			p = { 'Û', MAP_COLOR_WALL };
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
			p = { '/', MAP_COLOR_GRASS_LIGHT };
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
			p = { '÷', MAP_COLOR_WATER };
			break;
		case MAP_EMPTY:
		default:
			p = { ' ', MAP_COLOR_EMPTY };
			break;
	}

	if(i >= MAP_DOOR_MIN)
		p = { '+', MAP_COLOR_DOOR };

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
		map.info();
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

		tmp = map.getIntAt(player.x, player.y);

		if (tmp >= MAP_DOOR_MIN && (!isTraveling))
		{
			map.setFile(map.doors[VectorIndex(map.doors, tmp)].file);
			MapChange = true;
			isTraveling = true;
		} else
		{
			switch (tmp)
			{
				case MAP_SAND:
					Sleep(100);
					break;
				case MAP_WATER:
					Sleep(5);
					break;
				case MAP_OBJECTIVE:
					map.objectiveCount--;
					map.setIntAt(player.x, player.y, MAP_EMPTY);
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
			SetColor(MapLegend(map.getIntAt(player.oX, player.oY)).color);
			WriteAt(player.oX, player.oY, MapLegend(map.getIntAt(player.oX, player.oY)).ch);
		}
		if (!isDoor(tmp))
		{
			if (player.y <= 0 && !map.sNORTH.empty())
			{
				map.setFile(map.sNORTH);
				MapChange = true;
			}
			if (player.y >= map.GetHeight() - 1 && !map.sSOUTH.empty())
			{
				map.setFile(map.sSOUTH);
				MapChange = true;
			}
			if (player.x <= 0 && !map.sWEST.empty())
			{
				map.setFile(map.sWEST);
				MapChange = true;
			}
			if (player.x >= map.GetWidth() - 1 && !map.sEAST.empty())
			{
				map.setFile(map.sEAST);
				MapChange = true;
			}
		}
	}
}