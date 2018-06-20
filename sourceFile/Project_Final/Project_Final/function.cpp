#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>


#include"function.h"
#include "typing_speed.h"


//화면에 CLI를 출력하는 함수
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
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("====================================================================                     \n");
	printf("  입  력  >>\n");//gotoxy(14,25)
	printf("====================================================================                     \n");
	printf("\n");//오류 출력 영역(0,27)

}

//메뉴화면을 출력하고 사용자 입력을 대기하는 함수
char* printMenu(char title[][20], int count) {
	int i;
	int menu;
	gotoxy(22, 15);
	printf("========================\n");
	gotoxy(22, 16);
	printf("        메    뉴        \n");
	gotoxy(22, 17);
	printf("========================\n");
	
	//title 문자열 내의 데이터 출력
	for (i = 0; i < count; i++) {
		gotoxy(22, 18 + i);
		printf(" %d. %s\n", i + 1, title[i]);
	}
	gotoxy(22, 18 + count);
	printf("========================\n");

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
	gotoxy(76, 18);
	printf("%s", title[menu - 1]);
	gotoxy(0, 27);
	printf("                                                     ");
	return title[menu - 1];
}

//메뉴화면을 지우는 함수
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

//콘솔 상의 특정 xy좌표로 이동하는 함수
void gotoxy(int x, int y) {
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

//입력필드를 지우는 함수
void inputFieldClear() {
	//코드 보완 필요
	gotoxy(14, 25);
	printf("                                                  ");
}

//사용자로부터 입력받은 키를 반환하는 함수
char getKey() {
	if (kbhit()) { return getch(); }
	return '\0';
}

//사용자의 입력을 기다리는 함수
void userInput() {
	WORD_NODE *last_pos;
	WORD_NODE *temp = NULL;
	int i;
	int wd_locate = 0;
	int len;
	int flag = 0;
	
	while (hp != 0) {
		inputFieldClear();
		//사용자로부터 입력받은 값을 저장하고 화면에 출력하는 부분
		while (1) {
			//mtx.lock();
			user_text = getKey();
			//mtx.unlock();

			//엔터 입력시
			if (user_text == 13) {
				stop = system_clock::now();
				total_time = stop - start;

				type_pos = 14;
				wd_locate = 0;
				user_text = 0;
				flag = 0;

				break;
			//백스페이스 입력시
			}else if (user_text == 8) {
				if (wd_locate != 0) {
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
			//영문자 이외의 것 입력시
			else if (user_text != '\0') {
				//화면 출력용
				mtx.lock();
				gotoxy(type_pos,25);
				printf("%c", user_text);
				type_pos++;
				mtx.unlock();
				
				//실제 데이터 입력
				if (flag == 0) { start = system_clock::now(); flag = 1; }
				user_input[wd_locate] = user_text;
				wd_locate++;
			}
		}

		mtx.lock();
		//사용자의 입력과 글자 노드의 데이터를 비교하여 같다면 삭제하는 부분
		last_pos = Created;
		while (Created->previous != NULL) { Created = Created->previous; }
		
		while (Created != NULL){
			if (strcmp(Created->data, user_input) == 0) {
				//점수 계산 및 출력부
				score += (1000 + (combo * 100));
				combo++;
				printScore();
				printCombo();
				
				if(strstr(target_name, "korean") != NULL){
					tp_count = english_gettext(total_time, user_input);
					tp_result += tp_count;
					tp_ct++;
				}
				else {
					tp_count = korea_gettext(total_time, user_input);
					tp_result += tp_count;
					tp_ct++;
				}

				printTypeCount();
				
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
	
		//문자열을 길이에 맞게 비워줌
		len = strlen(user_input);
		if (strstr(target_name, "korean") == NULL) {
			for (i = 0; i < len * 2; i++) { user_input[i] = '\0'; }
		}
		else {
			for (i = 0; i < len; i++) { user_input[i] = '\0'; }
		}
		gotoxy(type_pos, 25);
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

		mtx.lock();
		//읽어온 데이터를 바탕으로 이중연결리스트를 생성하는 기능
		temp = (WORD_NODE*)malloc(sizeof(WORD_NODE));
		cource = rand() % (data_size / 2);
		if (cource % 2 == 0) {
			if (data_list->head->next == NULL) cource++;
		}
		else {
			if (data_list->head->prev == NULL) cource--;
		}

		//난수로 생성된 값이 짝수냐 홀수냐에 따라 이동방향 결정
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
		strcpy(temp->data, data_list->head->data);
		temp->pos_x = rand() % (MAX_WIDTH - strlen(temp->data));
		temp->pos_y = ST_HEIGHT;
		temp->next = NULL; temp->previous = NULL;

		//해당 노드가 배정된 위치에 문자열 출력
		gotoxy(temp->pos_x, temp->pos_y);
		printf("%s", temp->data);
		mtx.unlock();
		
		mtx.lock();
		if (Created == NULL) {
			Created = temp;
		}
		else {
			while (Created->next != NULL) { Created = Created->next; }
			Created->next = temp;
			temp->previous = Created;
			Created = temp;
		}
		mtx.unlock();

		//난이도 조정을 위한 Sleep함수
		Sleep(sleep_time);

		//노드의 위치정보를 y축으로 1씩 더해주는 기능
		mtx.lock();
		temp = Created;
		if (temp != NULL) {
			while (temp->previous != NULL) { temp = temp->previous; }
		}

		while (temp != NULL) {
			temp->pos_y++;
			if (temp->pos_y == MAX_HEIGHT) {
				hp -= 10;
				combo = 0;
				printHp();
				printCombo();
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

//사용자의 현재 체력을 출력해주는 기능
void printHp() {
	gotoxy(75, 19);
	printf("             ");
	gotoxy(75, 19);
	printf("H P : %2d",hp);
	
}

//사용자의 현재 점수을 출력해주는 기능
void printScore() {
	gotoxy(73, 20);
	printf("                                 ");
	gotoxy(73, 20);
	printf("Score : %6d", score);
	if (tp_ct != 0 && tp_ct % 8 == 0 && sleep_time > 0) { sleep_time -= 100; }
}

//사용자의 콤보수를 표시하는 함수
void printCombo() {
	gotoxy(77, 21);
	printf("%d combo", combo);
}

//사용자의 타자수를 출력해주는 함수
void printTypeCount() {
	gotoxy(73, 22);
	printf("타자수 : %.2f ", tp_count);
}


//게임 종료와 함께 취득 점수와 평균 타자수 출력
void gameover() {
	system("cls");
	printf("게임이 종료되었습니다.\n");
	printf("%s님의 점수 : %d점\n",user_name, score);
	printf("%s님의 평균타자수 : %.2f타\n", user_name, tp_result / (double)tp_ct);
	system("pause");
}