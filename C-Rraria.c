#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#include <time.h>

#define WIDTH  100
#define HEIGHT 50

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
int grassColor = 2;

char dirt[] = "%";
int dirtColor = 14;

char stone[] = "&";
int stoneColor = 8;

char air[] = " ";
int airColor = 3;

char player1[] = "@";
int player1Color = 11;

char player2[] = "@";
int player2Color = 5;

static int SEED = 0;

HANDLE stdOut;

// --------------------------------
void goToxy(int column, int line);
void writeChar(char *message);
void writeAt(int column, int line, char *character);

int noise2(int x, int y);
float lin_inter(float x, float y, float s);
float smooth_inter(float x, float y, float s);
float nosie2d(float x, float y);
float perlin2d(float x, float y, float freq, int depth);
// --------------------------------


int main()
{
	iVector playerPos1;
	iVector playerPos2;
	
	playerPos1.x = 5;
	playerPos1.y = 11;
	
	playerPos2.x = 8;
	playerPos2.y = 11;

	srand(time(NULL));
	SEED = rand()%1000000;

	system("color 0");
	system("cls");

	char tiles[LENGTH] = " ";
	int y, x, j;

	stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	for (y = 0; y < HEIGHT; ++y)
	{
		for (x = 0; x < WIDTH; ++x)
		{
			float generated = perlin2d(x, y, 0.2f, 4);
			float groundLevel = perlin2d(x, y, 0.05f, 2);
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
	
	for (y = 0; y < HEIGHT; ++y)
	{
		for (x = 0; x < WIDTH; ++x)
		{
			int numOfAirTiles = 0;

			if ((x > -1 && y > -1) && (x < WIDTH && y < HEIGHT))
			{
				for (j = 0; j < 3; ++j)
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

				for (j = 0; j < 3; ++j)
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
	writeAt(playerPos1.x, playerPos1.y, player1);
	
	SetConsoleTextAttribute(stdOut, player2Color);
	writeAt(playerPos2.x, playerPos2.y, player2);
	
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
	
	while (1) 
	{
		int comando;
		comando = _getch();
		
		if (comando == 's' || comando == 'S')
			{
				SetConsoleTextAttribute(stdOut, player1Color);
				writeAt(playerPos1.x, playerPos1.y, " ");
				playerPos1.y += 1;
				writeAt(playerPos1.x, playerPos1.y, player1);
			}
			
		if (comando == 'w' || comando == 'W')
			{
				SetConsoleTextAttribute(stdOut, player1Color);
				writeAt(playerPos1.x, playerPos1.y, " ");
				playerPos1.y -= 1;
				writeAt(playerPos1.x, playerPos1.y, player1);
			}
	
		if (comando == 'd' || comando == 'D')
			{
				SetConsoleTextAttribute(stdOut, player1Color);
				writeAt(playerPos1.x, playerPos1.y, " ");
				playerPos1.x += 1;
				writeAt(playerPos1.x, playerPos1.y, player1);
			}
		
		if (comando == 'a' || comando == 'A')
			{
				SetConsoleTextAttribute(stdOut, player1Color);
				writeAt(playerPos1.x, playerPos1.y, " ");
				playerPos1.x -= 1;
				writeAt(playerPos1.x, playerPos1.y, player1);
			}
			
		if (comando == 'k' || comando == 'K')
			{
				SetConsoleTextAttribute(stdOut, player2Color);
				writeAt(playerPos2.x, playerPos2.y, " ");
				playerPos2.y += 1;
				writeAt(playerPos2.x, playerPos2.y, player1);
			}
			
		if (comando == 'i' || comando == 'I')
			{
				SetConsoleTextAttribute(stdOut, player2Color);
				writeAt(playerPos2.x, playerPos2.y, " ");
				playerPos2.y -= 1;
				writeAt(playerPos2.x, playerPos2.y, player1);
			}
	
		if (comando == 'l' || comando == 'L')
			{
				SetConsoleTextAttribute(stdOut, player2Color);
				writeAt(playerPos2.x, playerPos2.y, " ");
				playerPos2.x += 1;
				writeAt(playerPos2.x, playerPos2.y, player1);
			}
		
		if (comando == 'j' || comando == 'J')
			{
				SetConsoleTextAttribute(stdOut, player2Color);
				writeAt(playerPos2.x, playerPos2.y, " ");
				playerPos2.x -= 1;
				writeAt(playerPos2.x, playerPos2.y, player1);
			}
			
		if (comando == 'q' || comando == 'Q')
			{
				exit(0);
			}	
	}

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
void writeChar(char *message)
{
	DWORD written = 0;
	WriteConsole(stdOut, message, strlen(message), &written, NULL);
}

/// <summary>Combines goToxy and write.</summary>
void writeAt(int column, int line, char *character)
{
	goToxy(column, line);
	writeChar(character);
}

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
