/**
  ******************************************************************************
  * @file    System.h
  * @author  Joshua Haden
  * @version V0.0.0
  * @date    20-MAY-2020
  * @brief   Header for System.cpp
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#pragma once

#include "PCH.h"

void gotoXY(int x, int y);

template<typename T>
inline void WriteAt(int x, int y, T data)
{
	gotoXY(x, y);
	std::cout << data;
}

void SetColor(int c);
void SetSize(int w, int h);

void ClearScreen();
void ShowConsoleCursor(bool showFlag);