#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <mutex>
#include <thread>
#include "ScoreList.h"
#include "functions.h"
#include "Textbaseconsole.h"
using namespace std;

//setcosole
#define screen_x 60
#define screen_y 25
#define lenght 100
const int width = 30;
const int height = 25;
const int wx = 30, hy = 25;

//food
#define count 100
COORD food[count];
int atefood;
int foodcount;
//wall
bool wallstat;
COORD wall[count];
int wallcount = 0;
//Oji
COORD oji[lenght];
int Tlength;
enum directions { UP, DOWN, LEFT, RIGHT, STOP };
directions dir;
COORD item[20];

int scoreitem = 90;
bool itemstat = false;

bool GameOn = true, mainmenu, modemenu, normalMode, obstructMode, play, Gameover = false, howtoplay, leaderboard, obsboard, challengeboard;
float speed;
int HP, score;
int mainpy = 15, modepy = 11, gameoverpy = 16, leaderpy = 12;
int runround = 0;
string name;

void game_setup()
{
	setcursor(0);
	srand(time(NULL));
	setMode();
	setConsole(screen_x, screen_y);
	mainmenu = true;
	play = false;
	modemenu = false;
	normalMode = false;
	obstructMode = false;
	leaderboard = false;
	obsboard = false;
	challengeboard = false;
	wallstat = false;
	itemstat = false;
	HP = 2;
	foodcount = 1;
	wallcount = 0;
	atefood = 3;
	Tlength = 1;
	speed = 80;
	score = 0;
	dir = STOP;
	init_oji();
	initfood();
	initwall();
}

void normalModegame()
{
	control_setting(); // w a s d f
	clear_buffer(); // clear
	addtail();
	oji_move();
	collisioncheck();
	selfhits();
	scorecount();
	gameplaypage();
	board();
	init_item();
	fill_food();
	fill_wall();
	fill_oji();
	fill_item();
	fill_buffer_to_console(); // fill
	Sleep(speed);
}

void control_setting()
{
	GetNumberOfConsoleInputEvents(rHnd, &numEvents);
	if (numEvents != 0) {
		INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
		ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
		for (DWORD i = 0; i < numEventsRead; ++i) {
			if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
				if (play == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						Sleep(300);
						normalMode = false;
						mainmenu = true;
						play = false;
						game_setup();
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'w' && dir != DOWN) {
						dir = UP;
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 's' && dir != UP) {
						dir = DOWN;
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'a' && dir != RIGHT) {
						dir = LEFT;
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'd' && dir != LEFT) {
						dir = RIGHT;
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'f') {
						dir = STOP;
					}
				}
				else if (mainmenu == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_UP)
					{
						if (mainpy != 15)
							mainpy -= 2;
					}
					else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
					{
						if (mainpy != 21)
							mainpy += 2;
					}
					else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					{
						if (mainpy == 15)
						{
							game_setup();
							mainmenu = false;
							modemenu = true;
						}
						else if (mainpy == 17)
						{
							mainmenu = false;
							leaderboard = true;
						}
						else if (mainpy == 19)
						{
							mainmenu = false;
							howtoplay = true;
						}
						else if (mainpy == 21)
						{
							exit(0);
						}
					}
				}
				else if (modemenu == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_UP)
					{
						if (modepy != 11)
							modepy -= 2;
					}
					else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
					{
						if (modepy != 15)
							modepy += 2;
					}
					else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					{
						if (modepy == 11)
						{
							//game_setup();
							wallstat = false;
							modemenu = false;
							play = true;
							normalMode = true;
						}
						else if (modepy == 13)
						{
							//game_setup();
							wallstat = true;
							modemenu = false;
							play = true;
							normalMode = true;
						}
						else if (modepy == 15)
						{
							modemenu = false;
							mainmenu = true;
						}
					}
				}
				else if (leaderboard == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_UP)
					{
						if (leaderpy != 12)
							leaderpy -= 2;
					}
					else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
					{
						if (leaderpy != 16)
							leaderpy += 2;
					}
					else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					{
						if (leaderpy == 12)
						{
							runround = 0;
							leaderboard = false;
							challengeboard = true;
						}
						else if (leaderpy == 14)
						{
							runround = 0;
							leaderboard = false;
							obsboard = true;
						}
						else if (leaderpy == 16)
						{
							runround = 0;
							leaderboard = false;
							mainmenu = true;
						}
					}
				}
				else if (obsboard == true || challengeboard == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					{
						if (obsboard == true) { obsboard = false; }
						else if (challengeboard == true) { challengeboard = false; }
						leaderboard = true;
					}
				}
				else if (Gameover == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					{
						mainmenu = true;
						Gameover = false;
						score = 0;
						resetgame();
					}
				}
				else if (howtoplay == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
					{
						mainmenu = true;
						howtoplay = false;
					}
				}
			}
		}
		delete[] eventBuffer;
	}
}

void fill_food()
{
	for (int i = 0; i < foodcount; i++)
	{
		consoleBuffer[food[i].X + screen_x * food[i].Y].Char.AsciiChar = '*';
		consoleBuffer[food[i].X + screen_x * food[i].Y].Attributes = 4;
	}
}

void fill_oji()
{
	consoleBuffer[oji[0].X + screen_x * oji[0].Y].Char.AsciiChar = '@';
	consoleBuffer[oji[0].X + screen_x * oji[0].Y].Attributes = 10;
	for (int i = 1; i < Tlength - 1; i++)
	{
		consoleBuffer[oji[i].X + screen_x * oji[i].Y].Char.AsciiChar = '@';
		consoleBuffer[oji[i].X + screen_x * oji[i].Y].Attributes = 2;
	}
}

void fill_wall()
{
	if (wallstat == true && wallcount > 0)
	{
		for (int i = 0; i < wallcount; i++)
		{
			consoleBuffer[wall[i].X + screen_x * wall[i].Y].Char.AsciiChar = '#';
			consoleBuffer[wall[i].X + screen_x * wall[i].Y].Attributes = 10;
		}
	}
}

void fill_item()
{
	if (itemstat == true)
	{
		consoleBuffer[item[0].X + screen_x * item[0].Y].Char.AsciiChar = '?';
		consoleBuffer[item[0].X + screen_x * item[0].Y].Attributes = 111;
	}
}

void init_item()
{
	if (scoreitem == 0)
	{
		SHORT x = 2 + (rand() % (wx - 3));
		SHORT y = 2 + (rand() % (hy - 4));
		for (int j = 0; j <= Tlength; j++)
		{
			for (int k = 0; k < foodcount; k++)
			{
				for (int l = 0; l < wallcount; l++)
					if ((x == oji[j].X && y == oji[j].Y) || (x == food[k].X && y == food[k].Y) || (x == wall[l].X && y == wall[l].Y))
					{
						bool found = true;
						while (found == true)
						{
							x = 1 + (rand() % (wx - 2));
							y = 1 + (rand() % (hy - 2));
							if ((x != oji[j].X && y != oji[j].Y) && (x != food[k].X && y != food[k].Y) && (x == wall[l].X && y == wall[l].Y))
								found = false;
						}
					}
			}
		}
		item[0] = { x, y };
		scoreitem = 90;
		itemstat = true;
	}
}

void initfood()
{
	for (int i = 0; i < foodcount; i++)
	{
		SHORT x = 1 + (rand() % (wx - 2));
		SHORT y = 1 + (rand() % (hy - 2));
		for (int j = 0; j <= Tlength; j++)
		{
			for (size_t k = 0; k <= wallcount; k++)
			{
				if ((x == oji[j].X && y == oji[j].Y) || (x == wall[k].X && y == wall[k].Y))
				{
					bool found = true;
					while (found == true)
					{
						x = 1 + (rand() % (wx - 2));
						y = 1 + (rand() % (hy - 2));
						if ((x != oji[j].X && y != oji[j].Y) && (x != wall[k].X && y != wall[k].Y))
							found = false;
					}
				}
			}
		}
		food[i] = { x, y };
	}
}

void initwall()
{
	if (wallstat == true && wallcount > 0)
	{
		for (int i = wallcount-1; i < wallcount; i++)
		{
			SHORT x = 2 + (rand() % (wx - 3));
			SHORT y = 2 + (rand() % (hy - 4));
			for (int j = 0; j < Tlength; j++)
			{
				for (int k = 0; k < foodcount; k++)
				{
					if ((x == oji[j].X && y == oji[j].Y) || (x == food[k].X && y == food[k].Y) || ((x - oji[0].X < 4) && (y - oji[0].Y < 4)))
					{
						bool found = true;
						while (found == true)
						{
							x = 1 + (rand() % (wx - 2));
							y = 1 + (rand() % (hy - 2));
							if ((x != oji[j].X && y != oji[j].Y) && (x != food[k].X && y != food[k].Y) && ((x - oji[0].X > 4) && (y - oji[0].Y > 4)))
							{
								found = false;
							}
						}
					}
				}
			}
			wall[i] = { x, y };
		}
	}
}

void init_oji()
{
	SHORT x = 10 + (rand() % (wx - 15));
	SHORT y = 5 + (rand() % (hy - 10));
	oji[0] = { x, y };
}

void oji_move()
{
	if (dir == UP) {
		if (oji[0].Y > 1)
		{
			oji[0].Y -= 1;
			Sleep(speed / 2);
		}
		else if (oji[0].Y <= 1 && HP > 1)
		{
			HP--;
			oji[0].Y = 23;
		}
		else if (oji[0].Y <= 1 && HP <= 1)
		{
			HP--;
			if (HP == 0) {
				thread g(Beep, 1175, 200);
				g.detach();
				Sleep(100);
				savescore();
				Gameover = true;
				normalMode = false;
				play = false;
				resetgame();
			}
		}
	}
	else if (dir == DOWN) {
		if (oji[0].Y < 23)
		{
			oji[0].Y += 1;
			Sleep(speed / 2);
		}
		else if (oji[0].Y >= 23 && HP > 1)
		{
			HP--;
			oji[0].Y = 1;
		}
		else if (oji[0].Y >= 23 && HP <= 1)
		{
			HP--;
			if (HP == 0) {
				thread g(Beep, 1175, 200);
				g.detach();
				Sleep(100);
				savescore();
				Gameover = true;
				normalMode = false;
				play = false;
				resetgame();
			}
		}
	}

	else if (dir == LEFT) {
		if (oji[0].X > 1)
		{
			oji[0].X -= 1;
		}
		else if (oji[0].X <= 1 && HP > 1)
		{
			HP--;
			oji[0].X = 28;
		}
		else if (oji[0].X <= 1 && HP <= 1)
		{
			HP--;
			if (HP == 0) {
				thread g(Beep, 1175, 200);
				g.detach();
				Sleep(100);
				savescore();
				Gameover = true;
				normalMode = false;
				play = false;
				resetgame();
			}
		}
	}

	else if (dir == RIGHT) {
		if (oji[0].X < width - 2)
		{
			oji[0].X += 1;
		}
		else if (oji[0].X >= 28 && HP > 1)
		{
			HP--;
			oji[0].X = 1;
		}
		else if (oji[0].X >= 28 && HP <= 1)
		{
			HP--;
			if (HP == 0) {
				thread g(Beep, 1175, 200);
				g.detach();
				Sleep(100);
				savescore();
				Gameover = true;
				normalMode = false;
				play = false;
				resetgame();
			}
		}
	}
}

void board()
{
	for (int i = 0; i < height; i++) //row
	{
		for (int j = 0; j < width; j++) // column
		{
			if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
			{
				consoleBuffer[j + screen_x * i].Char.AsciiChar = '#';
				consoleBuffer[j + screen_x * i].Attributes = 10;
			}
			else
				consoleBuffer[j + screen_x * i].Char.AsciiChar = ' ';
		}
	}
}

void collisioncheck()
{
	for (size_t i = 0; i < foodcount; i++)
	{
		if (oji[0].X == food[i].X && oji[0].Y == food[i].Y)
		{
			thread g(Beep, 636, 100);
			g.detach();
			if (Tlength == 1) {
				Tlength += 2;
			}
			else {
				Tlength++;
			}
			initfood();
			score += 10;
			scoreitem -= 10;
			if (speed != 50)
				speed -= 0.5;
			if (wallstat == true)
			{
				atefood--;
				if (atefood < 1)
				{
					wallcount++;
					initwall();
					atefood = 3;
				}
			}
		}
	}

	if (wallstat == true && wallcount > 0)
	{
		for (int i = 0; i < wallcount; i++)
		{
			if (oji[0].X == wall[i].X && oji[0].Y == wall[i].Y)
			{
				HP--;
				if (HP == 0)
				{
					thread w(Beep, 1175, 200);
					w.detach();
					savescore();
					play = false;
					normalMode = false;
					Gameover = true;
					resetgame();
				}
			}
		}
	}

	if (itemstat == true)
	{
		if (oji[0].X == item[0].X && oji[0].Y == item[0].Y)
		{
			thread a(Beep, 500, 100);
			a.detach();
			int peritem[10] = { 1,1,2,1,3,1,2,3,2,3 }; // 1 slow  2 hp 3 shorter
			int r = 1 + rand() % 10;
			if (peritem[r] == 1 && speed < 80)
				speed += 0.5;
			else if (peritem[r] == 2 && HP < 5)
				HP++;
			else if (peritem[r] == 3 && Tlength > 3)
				Tlength -= 2;
			itemstat = false;
		}
	}
}

void selfhits()
{
	for (int i = 1; i <= Tlength; i++)
	{
		if (oji[0].X == oji[i].X && oji[0].Y == oji[i].Y)
		{
			HP--;
			if (HP == 0)
			{
				Sleep(250);
				savescore();
				normalMode = false;
				play = false;
				Gameover = true;
				resetgame();
			}
		}
	}
}

void addtail()
{
	int fposX, fposY, sposX, sposY;
	if (dir != STOP)
	{
		fposX = oji[1].X;
		fposY = oji[1].Y;
		oji[1].X = oji[0].X;
		oji[1].Y = oji[0].Y;
		for (int i = 2; i < Tlength; i++)
		{
			sposX = oji[i].X;
			sposY = oji[i].Y;
			oji[i].X = fposX;
			oji[i].Y = fposY;
			fposX = sposX;
			fposY = sposY;
		}
	}
}

void savescore()
{
	if (wallstat == false)
	{
		ScoreList score_list("ChallengeScore.txt");
		score_list.loadFile();
		score_list.addEntry(name, score);
		score_list.saveFile();
	}
	else
	{
		ScoreList score_list("ObstructionScore.txt");
		score_list.loadFile();
		score_list.addEntry(name, score);
		score_list.saveFile();
	}
}


void myname()
{
	const char* nametext = "64011013 AMREE THAOWAN";
	SHORT px = 35;
	SHORT py = 24;
	for (size_t i = 0; i < 22; i++)
	{
		consoleBuffer[(px + i) + screen_x * py].Char.AsciiChar = nametext[i];
		consoleBuffer[(px + i) + screen_x * py].Attributes = 7;
	}
}

void gameplaypage()
{
	const char* a1 = "OJI'S";
	const char* mode11 = "MODE : CHALLENGE";
	const char* mode12 = "DO THE HIGHEST SCORE";
	const char* mode21 = "MODE : OBSTRUCTION";
	const char* mode22 = "DON'T HIT #";
	const char* l1 = "* = 10 point";
	const char* l2 = "# = Obstruction / Wall";
	const char* l3 = "? = Random item";
	const char* titlename = "USER NAME : ";
	const char* username = name.c_str();

	int py = 2;
	for (size_t i = 0; i < strlen(a1); i++)
	{
		consoleBuffer[30 + (((30 - strlen(a1)) / 2) + i) + screen_x * (py + 0)].Char.AsciiChar = a1[i];
		consoleBuffer[30 + (((30 - strlen(a1)) / 2) + i) + screen_x * (py + 0)].Attributes = 10;
	}
	if (wallstat == false) // challenge mode
	{

		for (size_t i = 0; i < strlen(mode11); i++)
		{
			consoleBuffer[30 + (((30 - strlen(mode11)) / 2) + i) + screen_x * (py + 2)].Char.AsciiChar = mode11[i];
			if (i < 5)
				consoleBuffer[30 + (((30 - strlen(mode11)) / 2) + i) + screen_x * (py + 2)].Attributes = 15;
			else
				consoleBuffer[30 + (((30 - strlen(mode11)) / 2) + i) + screen_x * (py + 2)].Attributes = 7;
		}

		for (size_t i = 0; i < strlen(mode12); i++)
		{
			consoleBuffer[30 + (((30 - strlen(mode12)) / 2) + i) + screen_x * (py + 4)].Char.AsciiChar = mode12[i];
			consoleBuffer[30 + (((30 - strlen(mode12)) / 2) + i) + screen_x * (py + 4)].Attributes = 7;
		}
	}
	else //obstruction mode
	{
		for (size_t i = 0; i < strlen(mode21); i++)
		{
			consoleBuffer[30 + (((30 - strlen(mode21)) / 2) + i) + screen_x * (py + 2)].Char.AsciiChar = mode21[i];
			if (i < 5)
				consoleBuffer[30 + (((30 - strlen(mode21)) / 2) + i) + screen_x * (py + 2)].Attributes = 15;
			else
				consoleBuffer[30 + (((30 - strlen(mode21)) / 2) + i) + screen_x * (py + 2)].Attributes = 7;
		}

		for (size_t i = 0; i < strlen(mode22); i++)
		{
			consoleBuffer[30 + (((30 - strlen(mode22)) / 2) + i) + screen_x * (py + 4)].Char.AsciiChar = mode22[i];
			if (i == 10)
				consoleBuffer[30 + (((30 - strlen(mode22)) / 2) + i) + screen_x * (py + 4)].Attributes = 10;
			else
				consoleBuffer[30 + (((30 - strlen(mode22)) / 2) + i) + screen_x * (py + 4)].Attributes = 7;
		}
	}
	for (size_t i = 0; i < strlen(l1); i++)
	{
		consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 15].Char.AsciiChar = l1[i];
		if (i == 0)
			consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 15].Attributes = 4;
		else
			consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 15].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(l2); i++)
	{
		consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 16].Char.AsciiChar = l2[i];
		if (i == 0)
			consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 16].Attributes = 10;
		else
			consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 16].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(l3); i++)
	{
		consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 17].Char.AsciiChar = l3[i];
		if (i == 0)
			consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 17].Attributes = 111;
		else
			consoleBuffer[30 + (((30 - strlen(l2)) / 2) + i) + screen_x * 17].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(titlename); i++)
	{
		consoleBuffer[30+(((30 - (strlen(titlename) + strlen(username))) / 2) + i) + screen_x * 22].Char.AsciiChar = titlename[i];
		consoleBuffer[30+(((30 - (strlen(titlename) + strlen(username))) / 2) + i) + screen_x * 22].Attributes = 10;
	}

	for (size_t i = 0; i < strlen(username); i++)
	{
		consoleBuffer[30+strlen(titlename) + (((30 - (strlen(titlename) + strlen(username))) / 2) + i) + screen_x * 22].Char.AsciiChar = username[i];
		consoleBuffer[30+strlen(titlename) + (((30 - (strlen(titlename) + strlen(username))) / 2) + i) + screen_x * 22].Attributes = 7;
	}
}

void scorecount()
{
	const char* scoretext = "SCORE = ";
	string s = to_string(score);
	const char* nchar = s.c_str();
	SHORT px = 39;
	SHORT py = 9;

	const char* health = "HP = ";
	string h = to_string(HP);
	const char* hchar = h.c_str();

	const char* tails = "TAIL LENGTH = ";
	string t = to_string(Tlength-2);
	const char* tchar = t.c_str();
	for (size_t i = 0; i < strlen(scoretext); i++)
	{
		consoleBuffer[(px + i) + screen_x * py].Char.AsciiChar = scoretext[i];
		consoleBuffer[(px + i) + screen_x * py].Attributes = 15;
	}
	for (size_t i = 0; i < strlen(nchar); i++)
	{
		consoleBuffer[(47 + i) + screen_x * py].Char.AsciiChar = nchar[i];
		consoleBuffer[(47 + i) + screen_x * py].Attributes = 2;
	}

	for (size_t i = 0; i < strlen(health); i++)
	{
		consoleBuffer[(px + i) + screen_x * (py + 2)].Char.AsciiChar = health[i];
		consoleBuffer[(px + i) + screen_x * (py + 2)].Attributes = 15;
	}
	for (size_t i = 0; i < strlen(hchar); i++)
	{
		consoleBuffer[(44 + i) + screen_x * (py + 2)].Char.AsciiChar = hchar[i];
		consoleBuffer[(44 + i) + screen_x * (py + 2)].Attributes = 4;
	}

	for (size_t i = 0; i < strlen(tails); i++)
	{
		consoleBuffer[(px + i) + screen_x * (py + 4)].Char.AsciiChar = tails[i];
		consoleBuffer[(px + i) + screen_x * (py + 4)].Attributes = 15;
	}
	if (Tlength <= 2)
	{
		consoleBuffer[53 + screen_x * (py + 4)].Char.AsciiChar = '0';
		consoleBuffer[53 + screen_x * (py + 4)].Attributes = 4;
	}
	else
	{
		for (size_t i = 0; i < strlen(tchar); i++)
		{
			consoleBuffer[(53 + i) + screen_x * (py + 4)].Char.AsciiChar = tchar[i];
			consoleBuffer[(53 + i) + screen_x * (py + 4)].Attributes = 4;
		}
	}
}

void menuascii()
{
	SHORT px = 20, py = 3;
	const char* ftext = "             ____";
	const char* stext = "            / . .\\";
	const char* dtext = "            \\  ---<";
	const char* gtext = "             \\  /";
	const char* htext = "   __________/ /";
	const char* jtext = "-=:___________/";

	for (size_t i = 0; i < strlen(ftext); i++)
	{
		consoleBuffer[(px + i) + screen_x * py].Char.AsciiChar = ftext[i];
		consoleBuffer[(px + i) + screen_x * py].Attributes = 10;
	}
	for (size_t i = 0; i < strlen(stext); i++)
	{
		consoleBuffer[(px + i) + screen_x * (py + 1)].Char.AsciiChar = stext[i];
		consoleBuffer[(px + i) + screen_x * (py + 1)].Attributes = 10;
	}
	for (size_t i = 0; i < strlen(dtext); i++)
	{
		consoleBuffer[(px + i) + screen_x * (py + 2)].Char.AsciiChar = dtext[i];
		consoleBuffer[(px + i) + screen_x * (py + 2)].Attributes = 10;
	}
	for (size_t i = 0; i < strlen(gtext); i++)
	{
		consoleBuffer[(px + i) + screen_x * (py + 3)].Char.AsciiChar = gtext[i];
		consoleBuffer[(px + i) + screen_x * (py + 3)].Attributes = 10;
	}
	for (size_t i = 0; i < strlen(htext); i++)
	{
		consoleBuffer[(px + i) + screen_x * (py + 4)].Char.AsciiChar = htext[i];
		consoleBuffer[(px + i) + screen_x * (py + 4)].Attributes = 10;
	}
	for (size_t i = 0; i < strlen(jtext); i++)
	{
		consoleBuffer[(px + i) + screen_x * (py + 5)].Char.AsciiChar = jtext[i];
		consoleBuffer[(px + i) + screen_x * (py + 5)].Attributes = 10;
	}

	const char* a1 = "OJI's";
	const char* start = "START";
	const char* leader = "LEADERBOARD";
	const char* how = "HOW TO PLAY";
	const char* Exits = "QUIT GAME";

	for (size_t i = 0; i < strlen(a1); i++)
	{
		consoleBuffer[(28 + i) + screen_x * (py + 8)].Char.AsciiChar = a1[i];
		consoleBuffer[(28 + i) + screen_x * (py + 8)].Attributes = 15;
	}

	for (size_t i = 0; i < strlen(start); i++)
	{
		consoleBuffer[(28 + i) + screen_x * (py + 12)].Char.AsciiChar = start[i];
		consoleBuffer[(28 + i) + screen_x * (py + 12)].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(leader); i++)
	{
		consoleBuffer[(25 + i) + screen_x * (py + 14)].Char.AsciiChar = leader[i];
		consoleBuffer[(25 + i) + screen_x * (py + 14)].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(how); i++)
	{
		consoleBuffer[(25 + i) + screen_x * (py + 16)].Char.AsciiChar = how[i];
		consoleBuffer[(25 + i) + screen_x * (py + 16)].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(Exits); i++)
	{
		consoleBuffer[(26 + i) + screen_x * (py + 18)].Char.AsciiChar = Exits[i];
		consoleBuffer[(26 + i) + screen_x * (py + 18)].Attributes = 7;
	}

	const char* nametext = "By 64011013/T. AMREE";
	SHORT tx = 35;
	SHORT ty = 24;
	for (size_t i = 0; i < 22; i++)
	{
		consoleBuffer[(tx + i) + screen_x * ty].Char.AsciiChar = nametext[i];
		consoleBuffer[(tx + i) + screen_x * ty].Attributes = 7;
	}
}

void modepage()
{
	const char* b1 = "MODES";
	const char* b2 = "CHALLENGE";
	const char* b3 = "OBSTRUCTION";
	const char* b4 = "BACK TO MAIN MENU";
	int py = 8;

	for (size_t i = 0; i < strlen(b1); i++)
	{
		consoleBuffer[(((60 - strlen(b1)) / 2) + i) + screen_x * py].Char.AsciiChar = b1[i];
		consoleBuffer[(((60 - strlen(b1)) / 2) + i) + screen_x * py].Attributes = 10;
	}

	for (size_t i = 0; i < strlen(b2); i++)
	{
		consoleBuffer[(((60 - strlen(b2)) / 2) + i) + screen_x * (py + 3)].Char.AsciiChar = b2[i];
		consoleBuffer[(((60 - strlen(b2)) / 2) + i) + screen_x * (py + 3)].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(b3); i++)
	{
		consoleBuffer[(((60 - strlen(b3)) / 2) + i) + screen_x * (py + 5)].Char.AsciiChar = b3[i];
		consoleBuffer[(((60 - strlen(b3)) / 2) + i) + screen_x * (py + 5)].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(b4); i++)
	{
		consoleBuffer[(((60 - strlen(b4)) / 2) + i) + screen_x * (py + 7)].Char.AsciiChar = b4[i];
		consoleBuffer[(((60 - strlen(b4)) / 2) + i) + screen_x * (py + 7)].Attributes = 7;
	}
}

void gameoverpage()
{
	SHORT py = 12;
	const char* text1 = "GAME OVER!!";
	const char* scoretext = "SCORE = ";
	const char* text3 = "BACK TO MAIN MENU";
	string s = to_string(score);
	const char* nchar = s.c_str();

	for (size_t i = 0; i < strlen(text1); i++)
	{
		consoleBuffer[(((60 - strlen(text1)) / 2) + i) + screen_x * (py - 4)].Char.AsciiChar = text1[i];
		consoleBuffer[(((60 - strlen(text1)) / 2) + i) + screen_x * (py - 4)].Attributes = 4;
	}

	for (size_t i = 0; i < strlen(scoretext); i++)
	{
		consoleBuffer[(((60 - (strlen(scoretext) + strlen(nchar))) / 2) + i) + screen_x * (py + 0)].Char.AsciiChar = scoretext[i];
		consoleBuffer[(((60 - (strlen(scoretext) + strlen(nchar))) / 2) + i) + screen_x * (py + 0)].Attributes = 7;
	}

	for (size_t i = 0; i < strlen(nchar); i++)
	{
		consoleBuffer[strlen(scoretext) + (((60 - (strlen(scoretext) + strlen(nchar))) / 2) + i) + screen_x * (py + 0)].Char.AsciiChar = nchar[i];
		consoleBuffer[strlen(scoretext) + (((60 - (strlen(scoretext) + strlen(nchar))) / 2) + i) + screen_x * (py + 0)].Attributes = 10;
	}

	for (size_t i = 0; i < strlen(text3); i++)
	{
		consoleBuffer[(((60 - strlen(text3)) / 2) + i) + screen_x * (py + 4)].Char.AsciiChar = text3[i];
		consoleBuffer[(((60 - strlen(text3)) / 2) + i) + screen_x * (py + 4)].Attributes = 7;
	}
}


void howtoplaypage()
{
	int py = 3;
	const char* topic = "HOW TO PLAY";
	const char* line1 = "KEY                         DETAIL";
	const char* line2 = " w                          go up";
	const char* line3 = " s                         go down";
	const char* line4 = " a                         go left";
	const char* line5 = "  d                         go right";
	const char* line6 = " f                          pause";
	const char* line7 = "BACK TO MAIN MENU";
	for (size_t i = 0; i < strlen(topic); i++)
	{
		consoleBuffer[(((60 - strlen(topic)) / 2) + i) + screen_x * (py + 0)].Char.AsciiChar = topic[i];
		consoleBuffer[(((60 - strlen(topic)) / 2) + i) + screen_x * (py + 0)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(line1); i++)
	{
		consoleBuffer[(((60 - strlen(line1)) / 2) + i) + screen_x * (py + 3)].Char.AsciiChar = line1[i];
		consoleBuffer[(((60 - strlen(line1)) / 2) + i) + screen_x * (py + 3)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(line2); i++)
	{
		consoleBuffer[(((60 - strlen(line2)) / 2) + i) + screen_x * (py + 5)].Char.AsciiChar = line2[i];
		consoleBuffer[(((60 - strlen(line2)) / 2) + i) + screen_x * (py + 5)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(line3); i++)
	{
		consoleBuffer[(((60 - strlen(line3)) / 2) + i) + screen_x * (py + 7)].Char.AsciiChar = line3[i];
		consoleBuffer[(((60 - strlen(line3)) / 2) + i) + screen_x * (py + 7)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(line4); i++)
	{
		consoleBuffer[(((60 - strlen(line4)) / 2) + i) + screen_x * (py + 9)].Char.AsciiChar = line4[i];
		consoleBuffer[(((60 - strlen(line4)) / 2) + i) + screen_x * (py + 9)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(line5); i++)
	{
		consoleBuffer[(((60 - strlen(line5)) / 2) + i) + screen_x * (py + 11)].Char.AsciiChar = line5[i];
		consoleBuffer[(((60 - strlen(line5)) / 2) + i) + screen_x * (py + 11)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(line6); i++)
	{
		consoleBuffer[(((60 - strlen(line6)) / 2) + i) + screen_x * (py + 13)].Char.AsciiChar = line6[i];
		consoleBuffer[(((60 - strlen(line6)) / 2) + i) + screen_x * (py + 13)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(line7); i++)
	{
		consoleBuffer[(((60 - strlen(line7)) / 2) + i) + screen_x * (py + 20)].Char.AsciiChar = line7[i];
		consoleBuffer[(((60 - strlen(line7)) / 2) + i) + screen_x * (py + 20)].Attributes = 7;
	}
}

void leaderboardpage()
{
	const char* topic = "LEADERBOARD";
	const char* l1 = "MODES";
	const char* l2 = "CHALLENGE";
	const char* l3 = "OBSTRUCTION";
	const char* l4 = "BACK TO MAIN MENU";
	int py = 8;
	for (size_t i = 0; i < strlen(topic); i++)
	{
		consoleBuffer[(((60 - strlen(topic)) / 2) + i) + screen_x * (py + 0)].Char.AsciiChar = topic[i];
		consoleBuffer[(((60 - strlen(topic)) / 2) + i) + screen_x * (py + 0)].Attributes = 10;
	}
	for (size_t i = 0; i < strlen(l1); i++)
	{
		consoleBuffer[(((60 - strlen(l1)) / 2) + i) + screen_x * (py + 2)].Char.AsciiChar = l1[i];
		consoleBuffer[(((60 - strlen(l1)) / 2) + i) + screen_x * (py + 2)].Attributes = 10;
	}
	for (size_t i = 0; i < strlen(l2); i++)
	{
		consoleBuffer[(((60 - strlen(l2)) / 2) + i) + screen_x * (py + 4)].Char.AsciiChar = l2[i];
		consoleBuffer[(((60 - strlen(l2)) / 2) + i) + screen_x * (py + 4)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(l3); i++)
	{
		consoleBuffer[(((60 - strlen(l3)) / 2) + i) + screen_x * (py + 6)].Char.AsciiChar = l3[i];
		consoleBuffer[(((60 - strlen(l3)) / 2) + i) + screen_x * (py + 6)].Attributes = 7;
	}
	for (size_t i = 0; i < strlen(l4); i++)
	{
		consoleBuffer[(((60 - strlen(l4)) / 2) + i) + screen_x * (py + 8)].Char.AsciiChar = l4[i];
		consoleBuffer[(((60 - strlen(l4)) / 2) + i) + screen_x * (py + 8)].Attributes = 7;
	}

}

void boardpage()
{
	control_setting();
	while (runround == 0)
	{
		clear_buffer();
		fill_buffer_to_console();
		gotoxy(0, 3);
		setcolor(6, 0);
		cout << " ========================LEADERBOARD========================";
		if (challengeboard == true)
		{
			gotoxy(22, 4);
			cout << "CHALLENGE'S MODE";
			gotoxy(0, 6);
			setcolor(10, 0);
			ScoreList score_list("ChallengeScore.txt");
			score_list.loadFile();
			score_list.printEntry(5);
			score_list.saveFile();
		}
		else if (obsboard == true)
		{
			gotoxy(22, 4);
			cout << "OBSTRUCTION'S MODE";
			gotoxy(0, 6);
			setcolor(10, 0);
			ScoreList score_list("ObstructionScore.txt");
			score_list.loadFile();
			score_list.printEntry(5);
			score_list.saveFile();
		}
		setcolor(6, 0);
		cout << endl << " ===========================================================";
		gotoxy(29, 24);
		cout << "PRESS ENTER : BACK TO MAIN MENU";
		runround++;
	}
}

void resetgame()
{
	wallstat = false;
	itemstat = false;
	HP = 2;
	foodcount = 1;
	wallcount = 0;
	atefood = 3;
	Tlength = 1;
	speed = 80;
	dir = STOP;
	init_oji();
	initfood();
	initwall();
}