#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"TeamProject.h"




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
		printf("파일오픈 실패!!\n");
		exit(0);
	}
	while (!feof(menu)) {		//menu
		fgets(tt, 50, menu);
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

	//fget으로 읽어 오는 과정에서 '\n' 같이 입력되어 삭제
	target_name[strlen(target_name) - 1] = '\0';
	strcat(target_name, ".txt");

	target = fopen(target_name, "r");
	if (target == NULL) {
		printf("파일오픈 실패!!\n");
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

//void textfile() {	//텍스트 파일을 출력해주는 함수
//	
//	Node_h *LS;
//	Node *p;
//	Menu *m;
//	Menu*temp;
//	FILE*fp1;
//	FILE*fp2;
//	FILE*fp3;
//	FILE*menu;
//
//	
//	p = NULL;
//	temp = NULL;
//	
//	char word[30];
//	char tt[20];
//	char select[20];
//	menu = fopen("title.txt", "r");
//	if (menu == NULL) {
//		printf("파일오픈 실패!!\n");
//	}
//	m = (Menu*)malloc(sizeof(Menu));
//	while (!feof(menu)) {		//menu
//
//		temp = (Menu*)malloc(sizeof(Menu));
//		fgets(tt, 50, menu);
//		strcpy(temp->Title, tt);
//		temp->next = m;
//		m = temp;
//		printf("%s\n", m->Title);
//	}
//	printf("1.korean   2.english  3.c_language\n");
//	scanf("%s", select);
//	if (strcmp(select , "korean")==0) {		//kor 
//		fp1 = fopen("korean.txt", "r");
//		if (fp1 == NULL) {
//			printf("파일오픈 실패!!\n");
//		}
//		while (!feof(fp1)) {
//			fscanf(fp1, "%s", word);
//			LS = createLinkedList_h();
//			p = searchNode(LS, word);
//			insertNode(LS, p, word);
//			printList(LS); getchar();
//		}
//		fclose(fp1);
//	}
//	if (strcmp(select, "english") == 0) {		//eng
//		fp2 = fopen("english.txt", "r");
//		if (fp2 == NULL) {
//			printf("파일오픈 실패!!\n");
//		}
//
//		while (!feof(fp2)) {
//			fscanf(fp2, "%s", word);
//			LS = createLinkedList_h();
//			p = searchNode(LS, word);
//			insertNode(LS, p, word);
//			printList(LS); getchar();
//
//		}
//		fclose(fp2);
//	}
//	if (strcmp(select, "c_language") == 0) {		//c_lang
//		fp3 = fopen("c_language.txt", "r");
//		if (fp3 == NULL) {
//			printf("파일오픈 실패!!\n");
//		}
//
//		while (!feof(fp3)) {
//			fscanf(fp3, "%s", word);
//			LS = createLinkedList_h();
//			p = searchNode(LS, word);
//			insertNode(LS, p, word);
//			printList(LS); getchar();
//
//		}
//		fclose(fp3);
//	}
//}
