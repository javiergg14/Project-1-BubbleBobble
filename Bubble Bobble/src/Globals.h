#pragma once

//Displaying scale factor
#define GAME_SCALE_FACTOR	2.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };

enum class GameState { MAIN_MENU, PLAYING1, SETTINGS, CREDITS, MAIN_TITLE, WINNING, LOSE};

//Main return values
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

//Log messages
#define LOG(text, ...) log(__FILE__, __LINE__, text, __VA_ARGS__);
void log(const char file[], int line, const char* text, ...);

//Window size
#define WINDOW_WIDTH    512
#define WINDOW_HEIGHT   480

//Game constants
#define MARGIN_GUI_Y	28
#define TILE_SIZE		16
#define LEVEL_WIDTH		32
#define LEVEL_HEIGHT	30

//Entities animation delay
#define ANIM_DELAY		8
#define ANIM_DELAY_ATTACK		3

