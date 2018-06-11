#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H



#define TITLE_FILE "title.txt"

#pragma warning(disable:4996)

typedef struct Node {
	char data[20];
	struct Node *next, *previous;
}NODE;

typedef struct Word_Node {
	char *data;
	int pos_x, pos_y;
}WORD_NODE;


static int hp;
static char user_input[20];
static NODE *created;

static char d_set[10][20] = { "danger", "world", "tensorflow", "playball", "mat", "taste", "phone", "notebook", "crazy", "final" };

int fileExist(const char *filename);
char* readTitle();
void printMenu(char title[][20], int count);
void readCustom(char *title);
void gotoxy(int x, int y);

void userInput();
void gamePlay();

#endif
