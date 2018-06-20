#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>


#include"function.h"
#include "typing_speed.h"


//ȭ�鿡 CLI�� ����ϴ� �Լ�
void printInterface() {
	printf("��D����               �ܡܡܡܡܡܡܡܡܡܡܡ�                                     \n");
	printf("                      �� �� �� �� �� �� ��  ��                                 ��    \n");
	printf("                      ��   ��    ��     ��  ��                              ����ᢺ \n");
	printf("                      �ܡܡܡܡܡܡܡܡܡܡܡ�                                ���   \n");
	printf("====================================================================   ------------------\n");
	printf("                                                                       %12s\n", user_name);
	printf("                                                                       ------------------\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                       ����������\n");
	printf("                                                                                         \n");
	printf("                                                                                         \n");//������ ü�¹� ���
	printf("                                                                                         \n");//���õ� �޴��� ���
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("====================================================================                     \n");
	printf("  ��  ��  >>\n");//gotoxy(14,25)
	printf("====================================================================                     \n");
	printf("\n");//���� ��� ����(0,27)

}

//�޴�ȭ���� ����ϰ� ����� �Է��� ����ϴ� �Լ�
char* printMenu(char title[][20], int count) {
	int i;
	int menu;
	gotoxy(22, 15);
	printf("========================\n");
	gotoxy(22, 16);
	printf("        ��    ��        \n");
	gotoxy(22, 17);
	printf("========================\n");
	
	//title ���ڿ� ���� ������ ���
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
			printf("��� : �޴��� ���� ���� ���ڸ� �Է����ּ���");
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

//�޴�ȭ���� ����� �Լ�
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

//�ܼ� ���� Ư�� xy��ǥ�� �̵��ϴ� �Լ�
void gotoxy(int x, int y) {
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

//�Է��ʵ带 ����� �Լ�
void inputFieldClear() {
	//�ڵ� ���� �ʿ�
	gotoxy(14, 25);
	printf("                                                  ");
}

//����ڷκ��� �Է¹��� Ű�� ��ȯ�ϴ� �Լ�
char getKey() {
	if (kbhit()) { return getch(); }
	return '\0';
}

//������� �Է��� ��ٸ��� �Լ�
void userInput() {
	WORD_NODE *last_pos;
	WORD_NODE *temp = NULL;
	int i;
	int wd_locate = 0;
	int len;
	int flag = 0;
	
	while (hp != 0) {
		inputFieldClear();
		//����ڷκ��� �Է¹��� ���� �����ϰ� ȭ�鿡 ����ϴ� �κ�
		while (1) {
			//mtx.lock();
			user_text = getKey();
			//mtx.unlock();

			//���� �Է½�
			if (user_text == 13) {
				stop = system_clock::now();
				total_time = stop - start;

				type_pos = 14;
				wd_locate = 0;
				user_text = 0;
				flag = 0;

				break;
			//�齺���̽� �Է½�
			}else if (user_text == 8) {
				if (wd_locate != 0) {
					//ȭ�� ��¿�
					mtx.lock();
					gotoxy(type_pos, 25);
					printf("\b");
					printf(" ");
					printf("\b");
					type_pos--;
					mtx.unlock();

					//���� ������ ����
					user_input[wd_locate] = '\0';
					wd_locate--;
				}
			}											
			//������ �̿��� �� �Է½�
			else if (user_text != '\0') {
				//ȭ�� ��¿�
				mtx.lock();
				gotoxy(type_pos,25);
				printf("%c", user_text);
				type_pos++;
				mtx.unlock();
				
				//���� ������ �Է�
				if (flag == 0) { start = system_clock::now(); flag = 1; }
				user_input[wd_locate] = user_text;
				wd_locate++;
			}
		}

		mtx.lock();
		//������� �Է°� ���� ����� �����͸� ���Ͽ� ���ٸ� �����ϴ� �κ�
		last_pos = Created;
		while (Created->previous != NULL) { Created = Created->previous; }
		
		while (Created != NULL){
			if (strcmp(Created->data, user_input) == 0) {
				//���� ��� �� ��º�
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

				//�ش���ġ�� �ؽ�Ʈ ����
				gotoxy(temp->pos_x, temp->pos_y);
				for (i = 0; i <= strlen(temp->data); i++) { printf(" "); }

				//��尡 �Ѱ��� ������ ���
				if (Created->next == NULL && Created->previous == NULL) {
					free(temp);
					Created = NULL;
					last_pos = NULL;
					break;			
				}
				//��尡 ù��° ����ϰ��
				else if (Created->previous == NULL) {
					Created->next->previous = NULL;
					Created = Created->next;
					free(temp);
					//last_pos = Created;
					break;
				}
				//��尡 ������ ����� ���
				else if (Created->next == NULL) {
					Created->previous->next = NULL;
					last_pos = Created->previous;
					free(temp);
					break;
				}
				//�׿� ��Ÿ ���
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
	
		//���ڿ��� ���̿� �°� �����
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

//���� ����� ���õ� �Լ�
void gamePlay() {
	WORD_NODE *temp = NULL;
	int i;
	int cource;
	srand(time(NULL));
	while (hp != 0) {

		mtx.lock();
		//�о�� �����͸� �������� ���߿��Ḯ��Ʈ�� �����ϴ� ���
		temp = (WORD_NODE*)malloc(sizeof(WORD_NODE));
		cource = rand() % (data_size / 2);
		if (cource % 2 == 0) {
			if (data_list->head->next == NULL) cource++;
		}
		else {
			if (data_list->head->prev == NULL) cource--;
		}

		//������ ������ ���� ¦���� Ȧ���Ŀ� ���� �̵����� ����
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

		//�ش� ��尡 ������ ��ġ�� ���ڿ� ���
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

		//���̵� ������ ���� Sleep�Լ�
		Sleep(sleep_time);

		//����� ��ġ������ y������ 1�� �����ִ� ���
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

//������� ���� ü���� ������ִ� ���
void printHp() {
	gotoxy(75, 19);
	printf("             ");
	gotoxy(75, 19);
	printf("H P : %2d",hp);
	
}

//������� ���� ������ ������ִ� ���
void printScore() {
	gotoxy(73, 20);
	printf("                                 ");
	gotoxy(73, 20);
	printf("Score : %6d", score);
	if (tp_ct != 0 && tp_ct % 8 == 0 && sleep_time > 0) { sleep_time -= 100; }
}

//������� �޺����� ǥ���ϴ� �Լ�
void printCombo() {
	gotoxy(77, 21);
	printf("%d combo", combo);
}

//������� Ÿ�ڼ��� ������ִ� �Լ�
void printTypeCount() {
	gotoxy(73, 22);
	printf("Ÿ�ڼ� : %.2f ", tp_count);
}


//���� ����� �Բ� ��� ������ ��� Ÿ�ڼ� ���
void gameover() {
	system("cls");
	printf("������ ����Ǿ����ϴ�.\n");
	printf("%s���� ���� : %d��\n",user_name, score);
	printf("%s���� ���Ÿ�ڼ� : %.2fŸ\n", user_name, tp_result / (double)tp_ct);
	system("pause");
}