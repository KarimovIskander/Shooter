#pragma once

#include <iostream>
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <chrono>
#include <utility>
#include <algorithm> 
#include <vector>

using namespace std;

extern int nScreenWidth;
extern int nScreenHeight;
extern int MapWidth;
extern int MapHeight;

extern float fPlayerX, fPlayerY, fPlayerA;
extern float fFOV;
extern float fDepth;
extern float fSpeed;

extern chrono::system_clock::time_point tp1;
extern chrono::system_clock::time_point tp2;

extern float fElapsedTime;



wstring GetMap(string path);

void Control(wstring &map);

extern float fDistanceToWall;
extern bool bHitWall;
extern bool bBound;

void FindDistance(wstring& map, int x);

void DrawLocation(wchar_t* screen, int x);

void DrawMap(wchar_t* screen, wstring& map);


