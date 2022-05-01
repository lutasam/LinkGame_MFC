#pragma once

typedef struct tagVertex {
	int row; //行
	int col; //列
	int info; //信息类
}Vertex;

typedef int* Vertices;
typedef int** AdjMatrix;

#define GAMEWND_WIDTH 800
#define GAMEWND_HEIGHT 600
#define BLANK -1
#define PLAY_TIMER_ID 1
#define GAME_LOSE -1
#define GAME_WIN 1
#define GAME_PLAYING 0