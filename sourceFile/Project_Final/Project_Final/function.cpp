#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>


#include"function.h"



//int fileExist(const char *filename) {
//	FILE *file;
//	if ((file = fopen(filename, "r")) == NULL) {
//		printf("파일을 읽는데 문제가 발생했습니다.\n");
//		return -1;
//	}
//	fclose(file);
//	return 1;
//}

//char* readTitle() {
//	FILE *t_file;
//	char title[10][20];
//	int count = 0;
//	char select[10];
//	char text[] = "FILE_IS_EMPTY\n";
//
//	if (fileExist(TITLE_FILE) == -1) exit(0);
//
//	t_file = fopen(TITLE_FILE, "r");
//
//	while (!feof(t_file)) {
//		fscanf(t_file, "%s", title[count]);
//		count++;
//	}
//	fclose(t_file);
//	printMenu(title, count);
//
//
//	printf("메뉴를 선택해주세요.\n>>");
//	scanf("%s", select);
//	for (int i = 0; i < count; i++) {
//		if (!strcmp(title[i], select)) return select;
//	}
//	return text;
//}



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
		/*if (!strcmp(title[i], "kor")){
			printf(" %d. 한 글\n", i + 1);
		}
		else if (!strcmp(title[i], "eng")) {
			printf(" %d. 영 어\n", i + 1);
		}
		else {
			printf(" %d. %s\n", i + 1, title[i]);
		}*/
		printf(" %d. %s\n", i + 1, title[i]);
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
	printHp();
	printScore();
	ready_to_game = 1;
}

//void readCustom(char* title) {
//	char c_filename[20];
//	FILE *c_file;
//
//	strcpy(c_filename, title);
//	strcat(c_filename, ".txt");
//
//	if (fileExist(c_filename) == -1) exit(0);
//
//	c_file = fopen(c_filename, "r");
//
//	fclose(c_file);
//}

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


char getKey() {
	if (kbhit()) { return getch(); }
	return '\0';
}


//노드 갯수에 따른 예외 처리 추가
void userInput() {
	WORD_NODE *last_pos;
	WORD_NODE *temp = NULL;
	int i;
	int wd_locate = 0;
	
	while (hp != 0) {
		inputFieldClear();
		//gotoxy(14, 25);
		while (1) {
			user_text = getKey();
			if (user_text == 13) {
				break;
			}else if (user_text == 8) {
				if (type_pos >= 14) {
					//화면 출력용
					mtx.lock();
					gotoxy(type_pos, 25);
					printf("\b");
					printf(" ");
					printf("\b");
					type_pos--;
					mtx.unlock();

					//실제 데이터 적용
					user_input[wd_locate] = '\0';
					wd_locate--;
				}
			}
			else if (user_text != '\0') {
				//화면 출력용
				mtx.lock();
				gotoxy(type_pos,25);
				printf("%c", user_text);
				type_pos++;
				mtx.unlock();
				
				//실제 데이터 입력
				user_input[wd_locate] = user_text;
				wd_locate++;
			}
			
		}
		type_pos = 14;
		wd_locate = 0;
		user_text = 0;

		gotoxy(1, 2);
		//사용자 입력 확인용
		printf("%s", user_input);

		mtx.lock();
		last_pos = Created;
		while (Created->previous != NULL) { Created = Created->previous; }
		
		//이부분에서 예외가 펑펑 발생
		while (Created != NULL){
			if (strcmp(Created->data, user_input) == 0) {
				score += 1000;
				printScore();

				temp = Created;

				//해당위치의 텍스트 삭제
				gotoxy(temp->pos_x, temp->pos_y);
				for (i = 0; i <= strlen(temp->data); i++) { printf(" "); }

				//노드가 한개만 존재할 경우
				if (Created->next == NULL && Created->previous == NULL) {
					free(temp);
					Created = NULL;
					last_pos = NULL;
					break;			
				}
				//노드가 첫번째 노드일경우
				else if (Created->previous == NULL) {
					Created->next->previous = NULL;
					Created = Created->next;
					free(temp);
					//last_pos = Created;
					break;
				}
				//노드가 마지막 노드일 경우
				else if (Created->next == NULL) {
					Created->previous->next = NULL;
					last_pos = Created->previous;
					free(temp);
					break;
				}
				//그외 기타 경우
				else {
					Created->previous->next = Created->next;
					Created->next->previous = Created->previous;
					Created = Created->next;
					free(temp);
					break;
				}
			}
			Created = Created->next;
			//break;
		}
		Created = last_pos;
		int len = strlen(user_input);
		gotoxy(1, 3);
		//문자열 길이 확인용
		printf("%d", len);

		if (strstr(target_name, "korean") == NULL) {
			for (i = 0; i < len * 2; i++) { user_input[i] = '\0'; }
		}
		else {
			for (i = 0; i < len; i++) { user_input[i] = '\0'; }
		}
		mtx.unlock();


		
	}
}

//게임 진행과 관련된 함수
void gamePlay() {
	WORD_NODE *temp = NULL;
	int i;
	int cource;
	srand(time(NULL));
	while (hp != 0) {

		temp = (WORD_NODE*)malloc(sizeof(WORD_NODE));

		cource = rand() % (data_size / 2);
		if (cource % 2 == 0) {
			if (data_list->head->next == NULL) cource++;
		}
		else {
			if (data_list->head->prev == NULL) cource--;
		}

		for (i = 0; i < cource; i++) {
			if (cource % 2 == 0) {
				if (data_list->head->next == NULL)  break; 
				data_list->head = data_list->head->next;
			}
			else {
				if (data_list->head->prev == NULL)  break; 
				data_list->head = data_list->head->prev;
			}
		}
		//strcpy(temp->data, d_set[rand() % 10]);
		strcpy(temp->data, data_list->head->data);
		temp->pos_x = rand() % (MAX_WIDTH - strlen(temp->data));
		temp->pos_y = ST_HEIGHT;
		temp->next = NULL; temp->previous = NULL;

		gotoxy(temp->pos_x, temp->pos_y);
		printf("%s", temp->data);
		//gotoxy(14, 25);

		mtx.lock();
		if (Created == NULL) {
			Created = temp;
			//head = *temp;
		}
		else {
			while (Created->next != NULL) { Created = Created->next; }
			Created->next = temp;
			temp->previous = Created;
			Created = temp;
		}
		mtx.unlock();


		Sleep(sleep_time);

		//연결리스트의 pos_y를 1씩 증가 + 화면 갱신(이건 별도의 함수로?)
		//증가하는 과정에서 y값이 일정값(20)에 도달하면 해당 노드를 삭제하고 체력 감소
		//strlen 이용하여 문자 지워주기 printf(" ")

		mtx.lock();
		temp = Created;
		if (temp != NULL) {
			while (temp->previous != NULL) { temp = temp->previous; }
		}

		while (temp != NULL) {
			temp->pos_y++;
			if (temp->pos_y == MAX_HEIGHT) {
				hp -= 10;
				printHp();
				gotoxy(temp->pos_x, temp->pos_y - 1);
				for (i = 0; i <= strlen(temp->data); i++) { printf(" "); }
				temp = temp->next;
				free(temp->previous);
				temp->previous = NULL;
			}
			else {
				gotoxy(temp->pos_x, temp->pos_y - 1);
				for (i = 0; i <= strlen(temp->data); i++) { printf(" "); }
				gotoxy(temp->pos_x, temp->pos_y);
				printf("%s", temp->data);
				temp = temp->next;
			}
		}
		mtx.unlock();
	}
	
}

void printHp() {
	gotoxy(75, 18);
	printf("H P : %2d",hp);
}

void printScore() {
	gotoxy(73, 19);
	printf("Score : %6d", score);
}