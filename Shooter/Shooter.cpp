#include "Algoritm.h"

#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <vector>
#include <utility>
#include <algorithm> 
using namespace std;



int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD DWBytesWritten = 0;

    wstring map;
	map = GetMap("Location1.txt");


    while (true) {//Цикл
		//
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		fElapsedTime = elapsedTime.count();
		//Управление
		Control(map);
		//Отрисовка
		for (int x = 0; x < nScreenWidth; x++) 
		{
			FindDistance(map, x);

			DrawLocation(screen, x);
		}

		DrawMap(screen, map);
		screen[((int)fPlayerY + 1) * nScreenWidth + (int)fPlayerX] = 'P';
		

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &DWBytesWritten);
    }
    return 0;
}


