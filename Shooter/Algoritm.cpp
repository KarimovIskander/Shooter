#include "Algoritm.h"

int nScreenWidth = 120;
int nScreenHeight = 40;
int MapWidth = 16;
int MapHeight = 16;

float fPlayerX = 8.0f, fPlayerY = 8.0f, fPlayerA = 0.0f;
float fFOV = 3.14159f / 4.0f;
float fDepth = 16.0f;
float fSpeed = 5.0f;

chrono::system_clock::time_point tp1;
chrono::system_clock::time_point tp2;

float fElapsedTime;

wstring GetMap(string path) {
	wstring map;

	ifstream file;
	file.open(path);

	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	if (file.is_open()) {
		string line;
		bool is_map = false;
		while (getline(file, line)) {
			if (line != "MAP") {
				map += converter.from_bytes(line);
			}
		}
	}
	return map;
}

void Control(wstring &map) {
	if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
		fPlayerA -= (1.0f) * fElapsedTime;
	}
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
		fPlayerA += (1.0f) * fElapsedTime;
	}
	if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
		fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
		fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

		if (map[(int)fPlayerY * MapWidth + (int)fPlayerX] == '#') {
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
		}
	}
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
		fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
		fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

		if (map[(int)fPlayerY * MapWidth + (int)fPlayerX] == '#') {
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
		}
	}
}

float fDistanceToWall = 0;
bool bHitWall = false;
bool bBound = false;

void FindDistance(wstring &map, int x) {
	fDistanceToWall = 0;
	bHitWall = false;
	bBound = false;

	float fRayAngle = (fPlayerA - fFOV / 2.0F) + ((float)x / (float)nScreenWidth) * fFOV;

	float fEyeX = sinf(fRayAngle);
	float fEyeY = cosf(fRayAngle);

	while (!bHitWall && fDistanceToWall < fDepth) {
		fDistanceToWall += 0.1f;

		int nPointX = (int)(fPlayerX + fEyeX * fDistanceToWall);
		int nPointY = (int)(fPlayerY + fEyeY * fDistanceToWall);

		if (nPointX < 0 || nPointX >= MapWidth || nPointY < 0 || nPointY >= MapHeight) {
			bHitWall = true;
			fDistanceToWall = fDepth;
		}
		else {
			if (map[nPointY * MapWidth + nPointX] == '#') {
				bHitWall = true;

				vector<pair<float, float>> p;

				for (int tx = 0; tx < 2; tx++) {
					for (int ty = 0; ty < 2; ty++) {
						float vy = (float)nPointY + ty - fPlayerY;
						float vx = (float)nPointX + tx - fPlayerX;

						float d = sqrt(vx * vx + vy * vy);
						float dot = (fEyeX * vx / d) + (fEyeY * vy / d);

						p.push_back(make_pair(d, dot));
					}
				}

				sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

				float fBound = 0.005;
				if (acos(p.at(0).second) < fBound)bBound = true;
				if (acos(p.at(1).second) < fBound)bBound = true;
				if (acos(p.at(2).second) < fBound)bBound = true;
			}
		}
	}
}

void DrawLocation(wchar_t* screen, int x) {
	int nCelling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
	int nFloor = nScreenHeight - nCelling;

	short nShade = ' ';
	if (fDistanceToWall <= fDepth / 4.0f) { nShade = 0x2588; }
	else if (fDistanceToWall < fDepth / 3.0f) { nShade = 0x2593; }
	else if (fDistanceToWall < fDepth / 2.0f) { nShade = 0x2592; }
	else if (fDistanceToWall < fDepth) { nShade = 0x2591; }
	else { nShade = ' '; }

	if (bBound) nShade = ' ';

	for (int y = 0; y < nScreenHeight; y++) {
		if (y <= nCelling) {
			screen[y * nScreenWidth + x] = ' ';
		}
		else if (y > nCelling && y <= nFloor) {
			screen[y * nScreenWidth + x] = nShade;
		}
		else {
			float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
			if (b < 0.25) { nShade = '#'; }
			else if (b < 0.5) { nShade = 'x'; }
			else if (b < 0.75) { nShade = '.'; }
			else if (b < 0.9) { nShade = '-'; }
			else { nShade = ' '; }
			screen[y * nScreenWidth + x] = nShade;
		}

	}
}


void DrawMap(wchar_t* screen, wstring& map) {
	swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);
	for (int nx = 0; nx < MapWidth; nx++) {
		for (int ny = 0; ny < MapHeight; ny++) {
			screen[(ny + 1) * nScreenWidth + nx] = map[ny * MapWidth + nx];
		}
	}
}








