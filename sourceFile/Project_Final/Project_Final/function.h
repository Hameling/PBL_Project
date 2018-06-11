#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H



#define TITLE_FILE "title.txt"

#define MAX_WIDTH 68	//ȭ�� x�� �ִ� ����
#define MAX_HEIGHT 24	//ȭ�� y�� �ִ� ����
#define ST_HEIGHT 5		//��� ���� ��ġ

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



//���� �Լ��� ���ÿ� ���Ǵ� ���� ����
extern char user_name[20];
extern int ready_to_game;

//��� ���Ͽ��� ���Ǵ� ���� ����
static int hp = 100;
static int score = 0;
static char user_input[20];
static int sleep_time = 3000;
//static NODE *created;
static WORD_NODE *Created = NULL;


static char d_set[10][20] = { "danger", "world", "tensorflow", "playball", "mat", "taste", "phone", "notebook", "crazy", "final" };

//��������� ����
int fileExist(const char *filename);
char* readTitle();
void readCustom(char *title);

//�޴� ��� ����
void printInterface();
char* printMenu(char title[][20], int count);
void clearMenu(int count);

//�ܼ� ȭ�� ��������
void gotoxy(int x, int y);
void inputFieldClear();

//�����������
void userInput();
void gamePlay();
void printHp();
void printScore();

#endif
