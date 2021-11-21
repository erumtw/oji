#include "source.h"

/*
 Oji's
 the snake game
 dev by Amree Thaowan
 64011013 KMITL
*/

int main()
{
	cout << "Enter your name : ";
	getline(cin, name);
	game_setup();
	while (GameOn)
	{
		while (leaderboard == true)
		{
			control_setting();
			clear_buffer();
			leaderboardpage();
			consoleBuffer[40 + screen_x * leaderpy].Char.AsciiChar = '<';
			consoleBuffer[40 + screen_x * leaderpy].Attributes = 3;;
			fill_buffer_to_console();
			Sleep(160);
		}
		while (challengeboard == true)
		{
			boardpage();
		}
		while (obsboard == true)
		{
			boardpage();
		}
		while (howtoplay == true)
		{
			control_setting();
			clear_buffer();
			howtoplaypage();
			consoleBuffer[40 + screen_x * 23].Char.AsciiChar = '<';
			consoleBuffer[40 + screen_x * 23].Attributes = 3;;
			fill_buffer_to_console();
			Sleep(160);
		}
		while (Gameover == true)
		{
			control_setting();
			clear_buffer();
			gameoverpage();
			consoleBuffer[40 + screen_x * gameoverpy].Char.AsciiChar = '<';
			consoleBuffer[40 + screen_x * gameoverpy].Attributes = 3;
			fill_buffer_to_console();
			Sleep(160);
		}
		while (mainmenu == true)
		{
			control_setting();
			clear_buffer(); // clear
			menuascii();
			consoleBuffer[37 + screen_x * mainpy].Char.AsciiChar = '<';
			consoleBuffer[37 + screen_x * mainpy].Attributes = 3;
			fill_buffer_to_console();
			Sleep(160);
		}
		while (modemenu == true)
		{
			control_setting();
			clear_buffer();
			modepage();
			consoleBuffer[39 + screen_x * modepy].Char.AsciiChar = '<';
			consoleBuffer[39 + screen_x * modepy].Attributes = 3;
			fill_buffer_to_console();
			Sleep(160);
		}
		while (normalMode == true)
		{
			normalModegame();
		}
	}
	return 0;
}