#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H



#define TITLE_FILE "title.txt"

#define MAX_WIDTH 68	//화면 x축 최대 길이
#define MAX_HEIGHT 24	//화면 y축 최대 길이
#define ST_HEIGHT 5		//출력 시작 위치

#pragma warning(disable:4996)

typedef struct Node {
	char data[20];
	struct Node *next, *previous;
}NODE;

typedef struct Word_Node {
	char data[20];
	int pos_x, pos_y;
	struct Word_Node *next, *previous;
}WORD_NODE;



//메인 함수와 동시에 사용되는 전역 변수
extern char user_name[20];
extern int ready_to_game;

//헤더 파일에서 사용되는 전역 변수
static int hp = 100;
static int score = 0;
static char user_input[20];
static int sleep_time = 3000;
//static NODE *created;
static WORD_NODE *Created = NULL;


static char d_set[10][20] = { "danger", "world", "tensorflow", "playball", "mat", "taste", "phone", "notebook", "crazy", "final" };

//파일입출력 관련
int fileExist(const char *filename);
char* readTitle();
void readCustom(char *title);

//메뉴 출력 관련
void printInterface();
char* printMenu(char title[][20], int count);
void clearMenu(int count);

//콘솔 화면 관리측면
void gotoxy(int x, int y);
void inputFieldClear();

//게임진행관련
void userInput();
void gamePlay();
void printHp();
void printScore();

#endif
