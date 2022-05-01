#pragma once

typedef struct tagVertex {
	int row; //��
	int col; //��
	int info; //��Ϣ��
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