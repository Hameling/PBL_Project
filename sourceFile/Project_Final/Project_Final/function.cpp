#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
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



void printInterface() {
	printf("『D조』               ●●●●●●●●●●●●                                     \n");
	printf("                      ● ㅅ ㅏ ㅅ ㅓ ㅂ ㅣ  ●                                 ▲    \n");
	printf("                      ●   ㄴ    ㅇ     ㅣ  ●                              ◀■■▶ \n");
	printf("                      ●●●●●●●●●●●●                                ■■   \n");
	printf("====================================================================   ------------------\n");
	printf("                                                                       %12s\n", user_name);
	printf("                                                                       ------------------\n");
	printf("                                                                       □□□□□□□□□\n");
	printf("                                                                       □□□■■■□□□\n");
	printf("                                                                       □□■■■■■□□\n");
	printf("                                                                       □□■■■■■□□\n");
	printf("                                                                       □□■■■■■□□\n");
	printf("                                                                       □□□■■■□□□\n");
	printf("                                                                       □□□□■□□□□\n");
	printf("                                                                       □□■■■■■□□\n");
	printf("                                                                       □■■■■■■■□\n");
	printf("                                                                       ■■■■■■■■■\n");
	printf("                                                                                         \n");
	printf("                                                                                         \n");//점수랑 체력바 출력
	printf("                                                                                         \n");//선택된 메뉴명 출력
	printf("                                                                                         \n");//처음에는 사용자 이름 ??? 로 출력??
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("====================================================================                     \n");
	printf("  입  력  >>\n");//gotoxy(14,25)
	printf("====================================================================                     \n");
	printf("\n");//오류 출력 영역(0,27)

}


char* printMenu(char title[][20], int count) {
	int i;
	int menu;
	gotoxy(22, 15);
	printf("========================\n");
	gotoxy(22, 16);
	printf("        메    뉴        \n");
	gotoxy(22, 17);
	printf("========================\n");
	
	for (i = 0; i < count; i++) {
		gotoxy(22, 18 + i);
		if (!strcmp(title[i], "kor")){
			printf(" %d. 한 글\n", i + 1);
		}
		else if (!strcmp(title[i], "eng")) {
			printf(" %d. 영 어\n", i + 1);
		}
		else {
			printf(" %d. %s\n", i + 1, title[i]);
		}
	}
	gotoxy(22, 18 + count);
	printf("========================\n");

	
	
	//지금은 간단히 숫자 입력으로
	//차후 텍스트 및 숫자 입력가능하게
	/*for (i = 0; i < count; i++) {	
		if(strcmp(menu, title[i]))
	}*/

	while (1) {
		gotoxy(14, 25);
		scanf("%d", &menu);

		if (menu > count || menu < 1) {
			gotoxy(0, 27);
			printf("경고 : 메뉴의 범위 안의 숫자를 입력해주세요");
			while (getchar() != '\n');
			inputFieldClear();
			continue;
		}
		else {
			inputFieldClear();
			break;
		}
	}
	return title[menu - 1];
}

void clearMenu(int count) {
	int i;
	for (i = 15; i <= (18 + count); i++) {
		gotoxy(22, i);
		printf("                         ");
	}
	ready_to_game = 1;
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

void gotoxy(int x, int y) {
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void inputFieldClear() {
	//코드 보완 필요
	gotoxy(14, 25);
	printf("                                                  ");
}

//노드 갯수에 따른 예외 처리 추가
void userInput() {
	WORD_NODE *temp;
	int i;
	while (hp != 0) {
		inputFieldClear();
		gotoxy(14, 25);
		scanf("%s", user_input);
		while (Created->previous != NULL) { Created = Created->previous; }
		
		//이부분에서 예외가 펑펑 발생
		while (Created != NULL){
			if (strcmp(Created->data, user_input) == 0) {
				temp = Created;
				Created->previous->next = Created->next;
				Created->next->previous = Created->previous;
				gotoxy(temp->pos_x, temp->pos_y);

				for (i = 0; i <= strlen(temp->data); i++) { printf(" "); }
				free(temp);
			}
			Created = Created->next;
		}
	}
}
void gamePlay() {
	WORD_NODE *temp = NULL;
	int i;
	srand(time(NULL));
	while (hp != 0) {
		temp = (WORD_NODE*)malloc(sizeof(WORD_NODE));
		strcpy(temp->data, d_set[rand() % 10]);
		temp->pos_x = rand() % (MAX_WIDTH - strlen(temp->data));
		temp->pos_y = ST_HEIGHT;
		temp->next = NULL; temp->previous = NULL;

		gotoxy(temp->pos_x, temp->pos_y);
		printf("%s", temp->data);
		gotoxy(14, 25);
		if (Created == NULL) {
			Created = temp;
			//head = *temp;
		}
		else {
			while (Created->next != NULL) { Created = Created->next; }
			Created->next = temp;
			temp->previous = Created;
		}


		Sleep(sleep_time);

		//연결리스트의 pos_y를 1씩 증가 + 화면 갱신(이건 별도의 함수로?)
		//증가하는 과정에서 y값이 일정값(20)에 도달하면 해당 노드를 삭제하고 체력 감소
		//strlen 이용하여 문자 지워주기 printf(" ")

		while (temp->previous != NULL) { temp = temp->previous; }

		while (temp != NULL) {
			temp->pos_y++;
			if (temp->pos_y == MAX_HEIGHT) {
				hp -= 10;
				printHp();
				gotoxy(temp->pos_x, temp->pos_y - 1);
				for (i = 0; i <= strlen(temp->data); i++) { printf(" "); }
				gotoxy(14, 25);
				temp = temp->next;
				free(temp->previous);
				temp->previous = NULL;
			}
			else {
				gotoxy(temp->pos_x, temp->pos_y - 1);
				for (i = 0; i <= strlen(temp->data); i++) { printf(" "); }
				gotoxy(temp->pos_x, temp->pos_y);
				printf("%s", temp->data);
				gotoxy(14, 25);
				temp = temp->next;
			}
		}
	}
	
}



void printHp() {
	gotoxy(1, 1);
	printf("H P : %2d",hp);
}