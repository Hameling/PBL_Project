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
//		printf("������ �дµ� ������ �߻��߽��ϴ�.\n");
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
//	printf("�޴��� �������ּ���.\n>>");
//	scanf("%s", select);
//	for (int i = 0; i < count; i++) {
//		if (!strcmp(title[i], select)) return select;
//	}
//	return text;
//}



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
	printf("                                                                                         \n");//ó������ ����� �̸� ??? �� ���??
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("                                                                                         \n");
	printf("====================================================================                     \n");
	printf("  ��  ��  >>\n");//gotoxy(14,25)
	printf("====================================================================                     \n");
	printf("\n");//���� ��� ����(0,27)

}


char* printMenu(char title[][20], int count) {
	int i;
	int menu;
	gotoxy(22, 15);
	printf("========================\n");
	gotoxy(22, 16);
	printf("        ��    ��        \n");
	gotoxy(22, 17);
	printf("========================\n");
	
	for (i = 0; i < count; i++) {
		gotoxy(22, 18 + i);
		/*if (!strcmp(title[i], "kor")){
			printf(" %d. �� ��\n", i + 1);
		}
		else if (!strcmp(title[i], "eng")) {
			printf(" %d. �� ��\n", i + 1);
		}
		else {
			printf(" %d. %s\n", i + 1, title[i]);
		}*/
		printf(" %d. %s\n", i + 1, title[i]);
	}
	gotoxy(22, 18 + count);
	printf("========================\n");

	
	
	//������ ������ ���� �Է�����
	//���� �ؽ�Ʈ �� ���� �Է°����ϰ�
	/*for (i = 0; i < count; i++) {	
		if(strcmp(menu, title[i]))
	}*/

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
	//�ڵ� ���� �ʿ�
	gotoxy(14, 25);
	printf("                                                  ");
}


char getKey() {
	if (kbhit()) { return getch(); }
	return '\0';
}


//��� ������ ���� ���� ó�� �߰�
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
			else if (user_text != '\0') {
				//ȭ�� ��¿�
				mtx.lock();
				gotoxy(type_pos,25);
				printf("%c", user_text);
				type_pos++;
				mtx.unlock();
				
				//���� ������ �Է�
				user_input[wd_locate] = user_text;
				wd_locate++;
			}
			
		}
		type_pos = 14;
		wd_locate = 0;
		user_text = 0;

		gotoxy(1, 2);
		//����� �Է� Ȯ�ο�
		printf("%s", user_input);

		mtx.lock();
		last_pos = Created;
		while (Created->previous != NULL) { Created = Created->previous; }
		
		//�̺κп��� ���ܰ� ���� �߻�
		while (Created != NULL){
			if (strcmp(Created->data, user_input) == 0) {
				score += 1000;
				printScore();

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
		int len = strlen(user_input);
		gotoxy(1, 3);
		//���ڿ� ���� Ȯ�ο�
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

//���� ����� ���õ� �Լ�
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

		//���Ḯ��Ʈ�� pos_y�� 1�� ���� + ȭ�� ����(�̰� ������ �Լ���?)
		//�����ϴ� �������� y���� ������(20)�� �����ϸ� �ش� ��带 �����ϰ� ü�� ����
		//strlen �̿��Ͽ� ���� �����ֱ� printf(" ")

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