#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include "Engine.h"



class OneLoneCoder_UltimateFPS : public olcConsoleGameEngine {
public:
	OneLoneCoder_UltimateFPS() {
		m_sAppName = L"FPS";
	}
	virtual bool OnUserCreate() {
		map += L"#########.......#########.......";
		map += L"#...............#...............";
		map += L"#.......#########.......########";
		map += L"#..............##..............#";
		map += L"#......##......##......##......#";
		map += L"#......##..............##......#";
		map += L"#..............##..............#";
		map += L"###............####............#";
		map += L"##.............###.............#";
		map += L"#............####............###";
		map += L"#..............................#";
		map += L"#..............##..............#";
		map += L"#..............##..............#";
		map += L"#...........#####...........####";
		map += L"#..............................#";
		map += L"###..####....########....#######";
		map += L"####.####.......######..........";
		map += L"#...............#...............";
		map += L"#.......#########.......##..####";
		map += L"#..............##..............#";
		map += L"#......##......##.......#......#";
		map += L"#......##......##......##......#";
		map += L"#..............##..............#";
		map += L"###............####............#";
		map += L"##.............###.............#";
		map += L"#............####............###";
		map += L"#..............................#";
		map += L"#..............................#";
		map += L"#..............##..............#";
		map += L"#...........##..............####";
		map += L"#..............##..............#";
		map += L"################################";

		spriteWall = new olcSprite(L"fps_wall1.spr");
		spriteLamp = new olcSprite(L"fps_lamp1.spr");
		spriteFireBall = new olcSprite(L"fps_fireball1.spr");

		fDepthBuffer = new float[ScreenWidth()];

		listObjects = {
			{8.5f,8.5f,0.0f,0.0f,false,spriteLamp},
			{7.5f,7.5f,0.0f,0.0f,false,spriteLamp},
			{10.5f,10.5f,0.0f,0.0f,false,spriteLamp},
		};

		return true;
	}
	virtual bool OnUserUpdate(float fElapsedTime) {
		// Управление
		if (m_keys[L'A'].bHeld) {
			fPlayerA -= (1.0f) * fElapsedTime;
		}
		if (m_keys[L'D'].bHeld) {
			fPlayerA += (1.0f) * fElapsedTime;
		}
		if (m_keys[L'W'].bHeld) {
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * MapWidth + (int)fPlayerX] == '#') {
				fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}
		if (m_keys[L'S'].bHeld) {
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * MapWidth + (int)fPlayerX] == '#') {
				fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}

		if (m_keys[L'Q'].bHeld) {
			fPlayerX -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += sinf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * MapWidth + (int)fPlayerX] == '#') {
				fPlayerX += cosf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}
		if (m_keys[L'E'].bHeld) {
			fPlayerX += cosf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= sinf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * MapWidth + (int)fPlayerX] == '#') {
				fPlayerX -= cosf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += sinf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}

		if (m_keys[VK_SPACE].bReleased) {
			sObject o;
			o.x = fPlayerX;
			o.y = fPlayerY;
			float fNoise = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f;
			o.vx = sinf(fPlayerA + fNoise) * 8.0f;
			o.vy = cosf(fPlayerA + fNoise) * 8.0f;
			o.sprite = spriteFireBall;
			o.bRemove = false;
			listObjects.push_back(o);
		}
		//Отрисовка
		for (int x = 0; x < ScreenWidth(); x++)
		{
			float fDistanceToWall = 0;
			bool bHitWall = false;
			bool bBound = false;

			float fRayAngle = (fPlayerA - fFOV / 2.0F) + ((float)x / (float)ScreenWidth()) * fFOV;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			float fSampleX = 0.0f;

			while (!bHitWall && fDistanceToWall < fDepth) {
				fDistanceToWall += 0.01f;

				int nPointX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nPointY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				if (nPointX < 0 || nPointX >= MapWidth || nPointY < 0 || nPointY >= MapHeight) {
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else {
					if (map[nPointY * MapWidth + nPointX] == '#') {
						bHitWall = true;

						float fBlockMidX = (float)nPointX + 0.5f;
						float fBlockMidY = (float)nPointY + 0.5f;

						float fTestPointX = fPlayerX + fEyeX * fDistanceToWall;
						float fTestPointY = fPlayerY + fEyeY * fDistanceToWall;

						float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX));

						if (fTestAngle >= -3.14159f * 0.25f && fTestAngle < 3.14159f * 0.25f) {
							fSampleX = fTestPointY - (float)nPointY;
						}
						if (fTestAngle >= 3.14159f * 0.25f && fTestAngle < 3.14159f * 0.75f) {
							fSampleX = fTestPointX - (float)nPointX;
						}
						if (fTestAngle < -3.14159f * 0.25f && fTestAngle >= -3.14159f * 0.75f) {
							fSampleX = fTestPointX - (float)nPointX;
						}
						if (fTestAngle >= 3.14159f * 0.75f || fTestAngle < -3.14159f * 0.75f) {
							fSampleX = fTestPointY - (float)nPointY;
						}
					}
				}
			}

			int nCelling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceToWall);
			int nFloor = ScreenHeight() - nCelling;

			fDepthBuffer[x] = fDistanceToWall;

			for (int y = 0; y < ScreenHeight(); y++) {
				if (y <= nCelling) {
					Draw(x, y, L' ');
				}
				else if (y > nCelling && y <= nFloor) {
					if (fDistanceToWall < fDepth) {
						float fSampleY = ((float)y - (float)nCelling) / ((float)nFloor - (float)nCelling);
						Draw(x, y, spriteWall->SampleGlyph(fSampleX, fSampleY), spriteWall->SampleColour(fSampleX, fSampleY));
					}
					else {
						Draw(x, y, PIXEL_SOLID, 0);
					}

				}
				else {

					Draw(x, y, PIXEL_SOLID, FG_DARK_BLUE);
				}

			}
		}
		for (auto& object : listObjects) {
			object.x += object.vx * fElapsedTime;
			object.y += object.vy * fElapsedTime;

			if (map.c_str()[(int)object.x * MapWidth + (int)object.y] == '#') {
				object.bRemove = true;
			}

			float fVecX = object.x - fPlayerX;
			float fVecY = object.y - fPlayerY;
			float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

			float fEyeX = sinf(fPlayerA);
			float fEyeY = cosf(fPlayerA);
			float fObjectAngle = atan2f(fEyeY, fEyeX) - atan2f(fVecY, fVecX);
			if (fObjectAngle < -3.14159f) {
				fObjectAngle += 2.0f * 3.14159f;
			}
			if (fObjectAngle > 3.14159f) {
				fObjectAngle -= 2.0f * 3.14159f;
			}
			bool bInPlayerFOV = fabs(fObjectAngle) < fFOV / 2.0f;

			if (bInPlayerFOV && fDistanceFromPlayer >= 0.5f && fDistanceFromPlayer < fDepth) {
				float fObjectCelling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceFromPlayer);
				float fObjectFloor = ScreenHeight() - fObjectCelling;
				float fObjectHeight = fObjectFloor - fObjectCelling;
				float fObjectAspectRatio = (float)object.sprite->nHeight / (float)object.sprite->nWidth;
				float fObjectWidth = fObjectHeight / fObjectAspectRatio;

				float fMiddleOfObjects = (0.5f*(fObjectAngle/(fFOV/2.0f))+0.5f)*(float)ScreenWidth();

				for (float lx = 0; lx < fObjectWidth; lx++) {
					for (float ly = 0; ly < fObjectHeight; ly++) {
						float fSampleX = lx / fObjectWidth;
						float fSampleY = ly/ fObjectHeight;
						wchar_t c = object.sprite->SampleGlyph(fSampleX, fSampleY);
						int nObjectColumn = (int)(fMiddleOfObjects + lx - (fObjectWidth / 2.0f));
						if (nObjectColumn >= 0 && nObjectColumn < ScreenWidth()) {
							if (c != L' ' && fDepthBuffer[nObjectColumn]>=fDistanceFromPlayer) {
								Draw(nObjectColumn, fObjectCelling + ly, c, object.sprite->SampleColour(fSampleX, fSampleY));
								fDepthBuffer[nObjectColumn] = fDistanceFromPlayer;
							}
							
						}
					}
				}
			}
		}
		listObjects.remove_if([](sObject& o) {return o.bRemove; });

		for (int nx = 0; nx < MapWidth; nx++) {
			for (int ny = 0; ny < MapHeight; ny++) {

				Draw(nx+1,ny+2, map[ny * MapWidth + nx]);
			}
		}

		Draw(1+(int)fPlayerY, 1+(int)fPlayerX, L'P');

		return true;
	}
private:

	int MapWidth = 32;				// World Dimensions
	int MapHeight = 32;

	// Create Map of world space # = wall block, . = space
	wstring map;

	float fPlayerX = 14.7f;			// Player Start Position
	float fPlayerY = 8.0f;
	float fPlayerA = 0.0f;			// Player Start Rotation
	float fFOV = 3.14159f / 4.0f;	// Field of View
	float fDepth = 16.0f;			// Maximum rendering distance
	float fSpeed = 5.0f;			// Walking Speed
	

	olcSprite *spriteWall;
	olcSprite *spriteLamp;
	olcSprite* spriteFireBall;

	float* fDepthBuffer = nullptr;

	struct sObject {
		float x;
		float y;
		float vx;
		float vy;
		bool bRemove;
		olcSprite* sprite;
	};



	list<sObject> listObjects;
};

int main()
{
	OneLoneCoder_UltimateFPS game;
	game.ConstructConsole(320, 240, 4, 4);
	game.Start();

	

	return 0;
}