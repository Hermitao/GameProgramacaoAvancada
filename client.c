#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <WinSock2.h>
#include <time.h>

#define WIDTH  64
#define HEIGHT 32

#define LENGTH WIDTH*HEIGHT

typedef struct iVector {
	int x;
	int y;
} iVector;

typedef struct fVector {
	float x;
	float y;
} fVector;

char tile[] = "#";

char grass[] = "#";
int grassColor = 1;

char dirt[] = "%";
int dirtColor = 4;

char stone[] = "&";
int stoneColor = 8;

char air[] = " ";
int airColor = 8;

char player1[] = "@";
int player1Color = 11;

HANDLE stdOut;

// --------------------------------
void goToxy(int column, int line);
void write(char *message);
void writeAt(int column, int line, char *character);

int noise2(int x, int y);
float lin_inter(float x, float y, float s);
float smooth_inter(float x, float y, float s);
float nosie2d(float x, float y);
float perlin2d(float x, float y, float freq, int depth);
// --------------------------------


int main()
{
	iVector playerPos;
	playerPos.x = 5;
	playerPos.y = 11;

	srand(time(NULL));

	system("color 0");
	system("cls");

	char tiles[LENGTH] = " ";

	stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			float generated = perlin2d(x, y, 0.1f, 4);
			float groundLevel = perlin2d(x, y, 0.08f, 2);
			float stoneLevel = perlin2d(x, y, 0.13f, 2);
			int iGroundLevel = (int)(groundLevel * 4.0f) - 2;
			int iStoneLevel = (int)(stoneLevel * 8.0f) - 4;
			if (y >= (22 + iStoneLevel))
			{
				if (generated > 0.48f)
				{
					SetConsoleTextAttribute(stdOut, stoneColor);
					writeAt(x, y, stone);
					tiles[x + (y*WIDTH)] = *stone;
				}
				else
				{
					SetConsoleTextAttribute(stdOut, airColor);
					writeAt(x, y, air);
					tiles[x + (y*WIDTH)] = *air;
				}			}
			else if (y >= (12 + iGroundLevel))
			{
				if (generated > 0.48f)
				{
					SetConsoleTextAttribute(stdOut, dirtColor);
					writeAt(x, y, dirt);
					tiles[x + (y*WIDTH)] = *dirt;
				}
				else
				{
					SetConsoleTextAttribute(stdOut, airColor);
					writeAt(x, y, air);
					tiles[x + (y*WIDTH)] = *air;
				}
			}
			else
			{
				SetConsoleTextAttribute(stdOut, airColor);
				writeAt(x, y, air);
				tiles[x + (y*WIDTH)] = *air;
			}

		}
	}
	
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			int numOfAirTiles = 0;

			if ((x > -1 && y > -1) && (x < WIDTH && y < HEIGHT))
			{
				for (int j = 0; j < 3; ++j)
				{
					if (tiles[x + ((y-1) * WIDTH) + j] == *air)
					{
						++numOfAirTiles;
					}
				}

				if (tiles[(x + (y * WIDTH)) - 1] == *air)
				{
					++numOfAirTiles;
				}

				if (tiles[(x + (y * WIDTH)) + 1] == *air)
				{
					++numOfAirTiles;
				}

				for (int j = 0; j < 3; ++j)
				{
					if (tiles[x + ((y+1) * WIDTH) + j] == *air)
					{
						++numOfAirTiles;
					}
				}

				if (numOfAirTiles > 0 && tiles[x + (y * WIDTH)] == *dirt && (y < 16 + (rand()%3 - 1)))
				{
					SetConsoleTextAttribute(stdOut, grassColor);
					writeAt(x, y, grass);
					tiles[x + (y * WIDTH)] = *grass;
				}
			}
		}
	}

	SetConsoleTextAttribute(stdOut, player1Color);
	writeAt(playerPos.x, playerPos.y, player1);

	while (1) {}

	return 0;
}

/// <summary>Go to position.</summary>
void goToxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	
	if (!SetConsoleCursorPosition(stdOut, coord))
	{
		printf("SetConsoleCursorPosition() failed - (");
		printf("%s", GetLastError()); 
		printf(")\n");
	}
}

/// <summary>Write text at cursor.</summary>
void write(char *message)
{
	DWORD written = 0;
	WriteConsole(stdOut, message, strlen(message), &written, NULL);
}

/// <summary>Combines goToxy and write.</summary>
void writeAt(int column, int line, char *character)
{
	goToxy(column, line);
	write(character);
}


static int SEED = 0;

static int hash[] = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                     185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                     9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                     70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                     203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                     164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                     228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                     232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                     193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                     101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                     135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                     114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};

int noise2(int x, int y)
{
    int tmp = hash[(y + SEED) % 256];
    return hash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s)
{
    return x + s * (y-x);
}

float smooth_inter(float x, float y, float s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}

float noise2d(float x, float y)
{
    int x_int = x;
    int y_int = y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = noise2(x_int, y_int);
    int t = noise2(x_int+1, y_int);
    int u = noise2(x_int, y_int+1);
    int v = noise2(x_int+1, y_int+1);
    float low = smooth_inter(s, t, x_frac);
    float high = smooth_inter(u, v, x_frac);
    return smooth_inter(low, high, y_frac);
}

float perlin2d(float x, float y, float freq, int depth)
{
    float xa = x*freq;
    float ya = y*freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;

    int i;
    for(i=0; i<depth; i++)
    {
        div += 256 * amp;
        fin += noise2d(xa, ya) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }

    return fin/div;
}
