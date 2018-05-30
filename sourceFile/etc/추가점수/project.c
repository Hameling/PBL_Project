#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

#define True 1
#define False 0

typedef struct node {
	char name[15];
	int age;
	struct node *next;
}NODE;

void printList(NODE *list, NODE *end);
void delList(NODE *list);
NODE* readFile();
NODE* findName(NODE *list, char* name);

int main() {
	char name[15];
	NODE *list = NULL;
	NODE *temp = NULL;

	printf("0을 입력하면 프로그램이 종료됩니다.\n");
	while (1) {
		printf("성명을 입력해주세요 : ");
		scanf("%s", name);
		if (strcmp(name, "0") == 0) {
			printf("프로그램을 종료합니다.\n");
			break;
		}
		list = readFile();
		temp = findName(list, name);
		if (temp == NULL) {
			printf("해당 이름이 존재하지 않습니다.\n");
		}
		else printList(list, temp);
		
		printf("\n");
	}

	delList(list);
	return 0;
}

void printList(NODE *list, NODE *end) {
	while (list != end) {
		printf("%s %d\n", list->name, list->age);
		list = list->next;
	}
}

void delList(NODE *list) {
	NODE *tmp;
	if (list == NULL) return;
	else {
		while (list != NULL) {
			tmp = list;
			list = list->next;
			free(tmp);
		}
	}
}

NODE* readFile() {
	FILE *data_file_1 = fopen("data_file_1.txt", "r");
	FILE *data_file_2 = fopen("data_file_2.txt", "r");
	NODE *temp;
	NODE *current = NULL, *follow = NULL, *list = NULL;
	char name[15];

	if (data_file_1 == NULL || data_file_2 == NULL) {
		printf("파일이 존재하지 않습니다.\n");
		return NULL;
	}
	while (!feof(data_file_1)) {
		temp = (NODE*)malloc(sizeof(NODE));
		fscanf(data_file_1, "%s %d", name, &temp->age);
		strcpy(temp->name, name);
		temp->next = NULL;

		current = follow = list;

		while ((current != NULL) && (current->age < temp->age)) {
			follow = current;
			current = current->next;
		}
		temp->next = current;
		if (current == list) list = temp;
		else follow->next = temp;
	}

	while (!feof(data_file_2)) {
		temp = (NODE*)malloc(sizeof(NODE));
		fscanf(data_file_2, "%s %d", name, &temp->age);
		strcpy(temp->name, name);
		temp->next = NULL;

		current = follow = list;

		while ((current != NULL) && (current->age < temp->age)) {
			follow = current;
			current = current->next;
		}
		temp->next = current;
		if (current == list) list = temp;
		else follow->next = temp;
	}

	fclose(data_file_1);
	fclose(data_file_2);
	return list;
}

NODE* findName(NODE *list, char* name) {
	NODE *temp = list;
	while (temp!= NULL){
		if (strcmp(temp->name, name) == 0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}