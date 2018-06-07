#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include"function.h"


int fileExist(const char *filename) {
	FILE *file;
	if ((file = fopen(filename, "r")) == NULL) {
		printf("파일을 읽는데 문제가 발생했습니다.\n");
		return -1;
	}
	fclose(file);
	return 1;
}
char* readTitle() {
	FILE *t_file;
	char title[10][20];
	int count = 0;
	char select[10];
	char text[] = "FILE_IS_EMPTY\n";

	if (fileExist(TITLE_FILE) == -1) exit(0);

	t_file = fopen(TITLE_FILE, "r");

	while (!feof(t_file)) {
		fscanf(t_file, "%s", title[count]);
		count++;
	}
	fclose(t_file);
	printMenu(title, count);


	printf("메뉴를 선택해주세요.\n>>");
	scanf("%s", select);
	for (int i = 0; i < count; i++) {
		if (!strcmp(title[i], select)) return select;
	}
	return text;
}


void printMenu(char title[][20], int count) {
	printf("=====================================\n");
	printf("메뉴\n");
	for (int i = 0; i < count; i++) {
		if (!strcmp(title[i], "kor")){
			printf("%d. 한글\n", i + 1);
		}
		else if (!strcmp(title[i], "eng")) {
			printf("%d. 영어\n", i + 1);
		}
		else {
			printf("%d. %s\n", i + 1, title[i]);
		}
	}
	printf("=====================================\n");
}


void readCustom(char* title) {
	char c_filename[20];
	FILE *c_file;

	strcpy(c_filename, title);
	strcat(c_filename, ".txt");

	if (fileExist(c_filename) == -1) exit(0);

	c_file = fopen(c_filename, "r");

	fclose(c_file);
}

void gotoxy(int x, int y){
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
