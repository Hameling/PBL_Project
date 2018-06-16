#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"TeamProject.h"




Node_h*createLinkedList_h(void) { //���� ���߿��Ḯ��Ʈ �����ϴ� �Լ�
	Node_h*LS;
	LS = (Node_h*)malloc(sizeof(Node_h));
	LS->head = NULL;
	return LS;
}

void printList(Node_h*LS) {		//���߿��Ḯ��Ʈ ����ϴ� �Լ�
	Node *p;
	printf("LS=(");
	p = LS->head;
	while (p != NULL) {
		printf("%s", p->data);
		p = p->next;
		if (p != NULL) {
			printf(",");
		}
		printf("\n");
	}
}

Node_h* insertNode(Node_h *LS, char *x) {
	Node* newNode;
	newNode = (Node*)malloc(sizeof(Node));
	strcpy(newNode->data, x);
	newNode->next = NULL;
	newNode->prev = NULL;
	if (LS->head == NULL) {
		LS->head = newNode;
	}
	else {
		while (LS->head->next != NULL) { LS->head = LS->head->next; }
		LS->head->next = newNode;
		newNode->prev = LS->head;
		LS->head = newNode;

		/*newNode->next = pre->next;
		pre->next = newNode;
		newNode->prev = pre;
		if (newNode->next != NULL) {
			newNode->next->prev = newNode;
		}*/
	}
	return LS;
}

Node* searchNode(Node_h*LS, char*x) { 
	Node*temp;
	temp = LS->head;
	while (temp != NULL) {
		if (strcmp(temp->data, x) == 0) {
			return temp;
		}
		else {
			temp = temp->next;
		}
		return temp;
	}
}


void readTitle() {
	FILE *menu;
	char tt[20];

	menu = fopen("title.txt", "r");
	if (menu == NULL) {
		printf("���Ͽ��� ����!!\n");
		exit(0);
	}
	while (!feof(menu)) {		//menu
		//fget �̿�� ���ͱ��� �Էµ� 
		//fgets(tt, 50, menu);
		fscanf(menu, "%s", tt);
		strcpy(title[title_count], tt);
		title_count++;
	}
	fclose(menu);
}

void readTarget(char target_name[]) {
	FILE *target;

	Node *p;
	Node_h *LS;
	char word[30];

	//fget���� �о� ���� �������� '\n' ���� �ԷµǾ� ����
	//target_name[strlen(target_name) - 1] = '\0';

	strcat(target_name, ".txt");

	//printf("target:%s", target_name);

	target = fopen(target_name, "r");
	if (target == NULL) {
		printf("���Ͽ��� ����!!\n");
		exit(0);
	}
	while (!feof(target)) {
		fscanf(target, "%s", word);
		if (data_list == NULL) {
			data_list = createLinkedList_h();
		}
		//data_set = searchNode(data_list, word);
		data_list = insertNode(data_list, word);
		//printList(LS); getchar();
		data_size++;
	}
	fclose(target);
}