#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>

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


int fileExist(char *filename);
char* readTitle();
void printMenu(char title[][10], int count);
void readCustom();



#endif
