#pragma once
#ifndef TEAMPROJECT_H
#define TEAMPROJECT_H

typedef struct Menu {
	char Title[20];
	struct Menu *next;
}Menu;

typedef struct Node {
	char data[20];
	struct Node *next;
	struct Node *prev;
}Node;

typedef struct {
	Node * head;
}Node_h;


//데이터를 전달받기 위한 전역변수 
extern char title[5][20];
extern int title_count;
extern Node *data_set;
extern Node_h *data_list;
extern int data_size;

Node_h*createLinkedList_h(void);
void printList(Node_h*LS);
Node_h* insertNode(Node_h *LS, char *x);
Node* searchNode(Node_h*LS, char*x);
//void textfile();

//파일 입출력 함수 구분
void readTitle();
void readTarget(char target_name[]);

#endif