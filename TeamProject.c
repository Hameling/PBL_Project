#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>



typedef struct Menu {
	char Title[50];
	struct Menu *next;
}Menu;

typedef struct Node {
	char data[10];
	struct Node *next;
	struct Node *prev;
}Node;

typedef struct {
	Node * head;
}Node_h;

Node_h*createLinkedList_h(void) { //공백 이중연결리스트 생성하는 함수
	Node_h*LS;
	LS = (Node_h*)malloc(sizeof(Node_h));
	LS->head = NULL;
	return LS;
}

void printList(Node_h*LS) {		//이중연결리스트 출력하는 함수
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

void insertNode(Node_h *LS, Node *pre, char *x) {
	Node*newNode;
	newNode = (Node*)malloc(sizeof(Node));
	strcpy(newNode->data, x);
	if (LS->head == NULL) {
		newNode->next = NULL;
		newNode->prev = NULL;
		LS->head = newNode;
	}
	else {
		newNode->next = pre->next;
		pre->next = newNode;
		newNode->prev = pre;
		if (newNode->next != NULL) {
			newNode->next->prev = newNode;
		}
	}
}

Node*searchNode(Node_h*LS, char*x) { 
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

void textfile(Node_h*LS,Node *p,Menu *m,Menu*temp,FILE*fp1,FILE*fp2,FILE*fp3,FILE*menu) {	//텍스트 파일을 출력해주는 함수
	p = NULL;
	temp = NULL;
	
	char word[100][10];
	char i[50];
	char select[20];
	menu = fopen("title.txt", "r");
	if (menu == NULL) {
		printf("파일오픈 실패!!\n");
	}
	m = (Menu*)malloc(sizeof(Menu));
	while (!feof(menu)) {		//menu

		temp = (Menu*)malloc(sizeof(Menu));
		fgets(i, 50, menu);
		strcpy(temp->Title, i);
		temp->next = m;
		m = temp;
		printf("%s\n", m->Title);
	}
	printf("1.korean   2.english  3.c_language\n");
	scanf("%s", select);
	if (strcmp(select , "korean")==0) {		//kor 
		fp1 = fopen("korean.txt", "r");
		if (fp1 == NULL) {
			printf("파일오픈 실패!!\n");
		}
		while (!feof(fp1)) {
			fscanf(fp1, "%s", word);
			LS = createLinkedList_h();
			p = searchNode(LS, word);
			insertNode(LS, p, word);
			printList(LS); getchar();
		}
		fclose(fp1);
	}
	if (strcmp(select, "english") == 0) {		//eng
		fp2 = fopen("english.txt", "r");
		if (fp2 == NULL) {
			printf("파일오픈 실패!!\n");
		}

		while (!feof(fp2)) {
			fscanf(fp2, "%s", word);
			LS = createLinkedList_h();
			p = searchNode(LS, word);
			insertNode(LS, p, word);
			printList(LS); getchar();

		}
		fclose(fp2);
	}
	if (strcmp(select, "c_language") == 0) {		//c_lang
		fp3 = fopen("c_language.txt", "r");
		if (fp3 == NULL) {
			printf("파일오픈 실패!!\n");
		}

		while (!feof(fp3)) {
			fscanf(fp3, "%s", word);
			LS = createLinkedList_h();
			p = searchNode(LS, word);
			insertNode(LS, p, word);
			printList(LS); getchar();

		}
		fclose(fp3);
	}
}

int main() {
	Node_h *LS;
	Node *p;
	Menu *m;
	Menu*temp;
	FILE*fp1;
	FILE*fp2;
	FILE*fp3;
	FILE*menu;
	
	textfile(&LS,&p,&m,&temp,&fp1,&fp2,&fp3,&menu);  //텍스트 파일을 출력해주는 함수
	
	return 0;
}