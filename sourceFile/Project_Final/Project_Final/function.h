#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include<mutex>
#include"TeamProject.h"



#define TITLE_FILE "title.txt"	//타이틀 파일 위치

#define MAX_WIDTH 68	//화면 x축 최대 길이
#define MAX_HEIGHT 24	//화면 y축 최대 길이
#define ST_HEIGHT 5		//출력 시작 위치

#pragma warning(disable:4996)

//typedef struct Node {
//	char data[20];
//	struct Node *next, *previous;
//}NODE;

typedef struct Word_Node {
	char data[30];
	int pos_x, pos_y;
	struct Word_Node *next, *previous;
}WORD_NODE;



//메인 함수와 동시에 사용되는 전역 변수
extern char user_name[20];
extern int ready_to_game;
extern char target_name[20];

//파일 입출력에 쓰일 변수
extern Node *data_set;
extern Node_h *data_list;
extern int data_size;

//헤더 파일에서 사용되는 전역 변수
static int hp = 100;
static int score = 0;
static int combo = 0;
static int type_pos = 14;
static char user_input[20] = "";
static char user_text;
static int sleep_time = 1000;
static WORD_NODE *Created = NULL;


//쓰레드 자원공유 문제 해결용
static std::mutex mtx;

//메뉴 출력 관련
void printInterface();
char* printMenu(char title[][20], int count);
void clearMenu(int count);

//콘솔 화면 관리측면
void gotoxy(int x, int y);
void inputFieldClear();

//사용자 입력 관련 함수
char getKey();

//게임진행관련
void userInput();
void gamePlay();
void printHp();
void printScore();
void printCombo();

#endif
