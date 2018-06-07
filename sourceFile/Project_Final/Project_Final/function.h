#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H



#define TITLE_FILE "title.txt"

#pragma warning(disable:4996)

typedef struct Node {
	char *data;
	struct Node *next, *previous;
}NODE;

typedef struct Word_Node {
	char *data;
	int pos_x, pos_y;
}WORD_NODE;


int fileExist(const char *filename);
char* readTitle();
void printMenu(char title[][20], int count);
void readCustom(char *title);
void gotoxy(int x, int y);

#endif
