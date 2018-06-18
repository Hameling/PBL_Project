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


//�����͸� ���޹ޱ� ���� �������� 
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

//���� ����� �Լ� ����
void readTitle();
void readTarget(char target_name[]);

#endif