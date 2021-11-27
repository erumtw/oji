#pragma once

int setConsole(int, int);
int setMode();
void fill_buffer_to_console();
void clear_buffer();
void init_oji();
void initfood();
void initwall();
void init_item();
void fill_oji();
void fill_food();
void fill_wall();
void fill_item();
void oji_move();
void board();
void collisioncheck();
void addtail();
void setcursor(bool);
void selfhits();
void game_setup();
void control_setting();
void myname();
void scorecount();
void normalModegame();
void menuascii();
void modepage();
void gameoverpage();
void howtoplaypage();
void gameplaypage();
void leaderboardpage();
void savescore();
void boardpage();
void deathsound();
void resetgame();

/*
thread q(Beep, 1568, 200);
q.detach();
thread a(Beep, 1568, 200);
a.detach();
thread b(Beep, 1568, 200);
b.detach();
thread c(Beep, 1245, 100);
c.detach();
thread d(Beep, 1397, 200);
d.detach();
thread e(Beep, 1397, 200);
e.detach();
thread f(Beep, 1397, 200);
f.detach();
thread g(Beep, 1175, 1000);
g.detach();
*/