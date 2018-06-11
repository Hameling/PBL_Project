#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<Windows.h>
#include"function.h"


int fileExist(const char *filename) {
	FILE *file;
	if ((file = fopen(filename, "r")) == NULL) {
		printf("������ �дµ� ������ �߻��߽��ϴ�.\n");
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


	printf("�޴��� �������ּ���.\n>>");
	scanf("%s", select);
	for (int i = 0; i < count; i++) {
		if (!strcmp(title[i], select)) return select;
	}
	return text;
}



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
		if (!strcmp(title[i], "kor")){
			printf(" %d. �� ��\n", i + 1);
		}
		else if (!strcmp(title[i], "eng")) {
			printf(" %d. �� ��\n", i + 1);
		}
		else {
			printf(" %d. %s\n", i + 1, title[i]);
		}
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
	//�ڵ� ���� �ʿ�
	gotoxy(14, 25);
	printf("                                                  ");
}

//��� ������ ���� ���� ó�� �߰�
void userInput() {
	WORD_NODE *temp;
	int i;
	while (hp != 0) {
		inputFieldClear();
		gotoxy(14, 25);
		scanf("%s", user_input);
		while (Created->previous != NULL) { Created = Created->previous; }
		
		//�̺κп��� ���ܰ� ���� �߻�
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

		//���Ḯ��Ʈ�� pos_y�� 1�� ���� + ȭ�� ����(�̰� ������ �Լ���?)
		//�����ϴ� �������� y���� ������(20)�� �����ϸ� �ش� ��带 �����ϰ� ü�� ����
		//strlen �̿��Ͽ� ���� �����ֱ� printf(" ")

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