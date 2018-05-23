#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"tag_ID_position.h"
#pragma warning(disable:4996)

//RFID �±� ������ �����ϱ� ���� ����ü
typedef struct taginfo {
	char id[27];
	float rssi;
	double identifiedTime;
	struct taginfo *next;
}TAGINFO;

//RFID �±� ������ Ȱ���� ���� ���Ḯ��Ʈ�� ó���� ���� �����ϱ� ���� ����ü
typedef struct List {
	 TAGINFO *first, *end;
}LIST;

//RSSI �� ������ ���Ľ�Ű�� ���� ����ü
typedef struct sortedList {
	char id[27];
	int pos[2];
	float rssi;
	struct sortedList *previous, *next;
}SORTEDLIST;

//���ĵ� ����ü�� Ȱ���� ���� ���Ḯ��Ʈ�� ó���� ���� �����ϴ� ����ü
typedef struct S_List {
	SORTEDLIST *first, *end;
}S_LIST;

void initList(TAGINFO *list);											//TagInfo ���Ḯ��Ʈ �ʱ�ȭ�� ���� �Լ�
void insertList(LIST **list, TAGINFO *input, int p);					//TagInfo ���Ḯ��Ʈ�� ������ �����ϱ� ���� �Լ�
S_LIST* insertSortList(S_LIST *list, float rssi, int p);				//SortedList ���Ḯ��Ʈ�� ������ ������������ �����ϱ� ���� �Լ�

void initMenu();														//�޴� ����ȭ���� ����ϱ� ���� �Լ� 
void readFile(LIST *list[]);											//������ �о�� �� �±��� �������� ���Ḯ��Ʈ�� �����ϴ� �Լ�

int referTagAnalysis(LIST *list[], S_LIST **sorted, int flag);			//Reference Tag �м� �� ������ ��� ���
double targetTagAnalysis(LIST *list[]);									//Target Tag �м� �� ������ ��� ���
void defineTagetPos(S_LIST *list, double avg_rssi, int rf_count);		//Referenence Tag�� RSSI���� �������� Target Tag�� ��ġ�� �߷��ϴ� ���
void printInfo(SORTEDLIST list[], int size, int t_x, int t_y);			//�±� ��ġ���� ���

int main() {
	char input;
	LIST *data_set[61] = { NULL };
	S_LIST *s_list = NULL;
	double avg_rssi;
	int rf_count;

	readFile(data_set);
	rf_count = referTagAnalysis(data_set, &s_list,0);
	avg_rssi = targetTagAnalysis(data_set);
	system("cls");
	
	initMenu();
	while (1) {
		printf(">>");
		scanf("%c", &input);

		if (input != 'A' && input != 'B' && input != 'C' && input != 'D') {
			printf("Unknown Input\n");
			printf("Please Select the A to D\n");
			while (getchar() != '\n');
			continue;

		}else if (input == 'A') {
			printf("A. Reference Tag Analysis\n");
			referTagAnalysis(data_set, s_list,1);
		}
		else if (input == 'B') {
			printf("B. Target Tag Analysis\n");
			targetTagAnalysis(data_set);
		}
		else if (input == 'C') {
			printf("C. Estimation of Target Position\n");
			defineTagetPos(s_list, avg_rssi, rf_count);
		}
		else if (input == 'D') {
			printf("D. Quit\n");
			break;
		}
		while (getchar() != '\n');
		system("pause");
		system("cls");
		initMenu();
	}
	printf("���α׷��� ���� ����Ǿ����ϴ�.\n");
	return 0;
}

//TagInfo ���Ḯ��Ʈ �ʱ�ȭ�� ���� �Լ�
void initList(TAGINFO *list) {
	strcpy(list->id, "");
	list->rssi = 0.0;
	list->identifiedTime = 0.0;
	list->next = NULL;
}

//TagInfo ���Ḯ��Ʈ�� ������ �����ϱ� ���� �Լ�
void insertList(LIST **list, TAGINFO *input, int p) {
	if (list[p]== NULL) {
		list[p] = (LIST*)malloc(sizeof(LIST));
		list[p]->first = input;
		list[p]->end = list[p]->first;
	}
	else {
		list[p]->end->next = input;
		list[p]->end = input;
	}
}

//SortedList ���Ḯ��Ʈ�� ������ ������������ �����ϱ� ���� �Լ�
//���߿��Ḯ��Ʈ�� ������ SortedList�� �ʱ�ȭ �� ���ĵ� ���·� ����
S_LIST* insertSortList(S_LIST *list, float rssi, int p) {
	SORTEDLIST *head;
	int flag = 0;

	SORTEDLIST *temp = (SORTEDLIST*)malloc(sizeof(SORTEDLIST));
	strcpy(temp->id, referenceIDs[p - 1]);
	temp->rssi = rssi;
	temp->pos[0] = referecePoints[p - 1][0];
	temp->pos[1] = referecePoints[p - 1][1];
	temp->next = NULL;
	temp->previous = NULL;

	if (list == NULL) {
		list = (S_LIST*)malloc(sizeof(S_LIST));
		list->first = temp;
		list->end = temp;
	}
	else {
		head = list->first;
		while (flag != 1){
			if (head->rssi >= temp->rssi) {
				if (head->previous != NULL) {
					head->previous->next = temp;
					temp->previous = head->previous;
					temp->next = head;
					head->previous = temp;
					flag = 1;
				}
				else {
					head->previous = temp;
					temp->next = head;
					list->first = temp;
					flag = 1;
				}
			}
			else {
				if (head->next != NULL) {
					head = head->next;
				}
				else {
					head->next = temp;
					temp->previous = head;
					list->end = temp;
					flag = 1;
				}
			}
		}
	}
	return list;
}

//�޴� ����ȭ���� ����ϱ� ���� �Լ�
void initMenu() {
	printf("=============================================================\n");
	printf("   RFID Tag Information Analysis based Localization System\n");
	printf("=============================================================\n");
	printf("  A. Reference Tag Analysis\n");
	printf("  B. Target Tag Analysis\n");
	printf("  C. Estimation of Target Position\n");
	printf("  D. Quit\n");
	printf("=============================================================\n");
}

//������ �о�� �� �±��� �������� ���Ḯ��Ʈ�� �����ϴ� �Լ�
//������ �����͸� ���� �� �о�� ���Ḯ��Ʈ�� �ʿ��� ������ ����
void readFile(LIST *list[]) {
	TAGINFO *temp_data;
	char readData[150];
	char *pos, *pos2;
	char tag_id[27];
	char rssi[5] = "";
	char time[13];

	int i;
	int count = 1;

	FILE *RFID_DATA = fopen("RFID_Data.txt", "r");

	if (RFID_DATA != NULL) {
		while (!feof(RFID_DATA)) {
			temp_data = (TAGINFO*)malloc(sizeof(TAGINFO));
			initList(temp_data);

			fgets(readData, 150, RFID_DATA);
			
			//Tag_ID
			pos = strstr(readData, "tag_id=") + 7;
			strncpy(temp_data->id, pos, 26);
			temp_data->id[26] = '\0';

			//rssi �� �Ź� �ʱ�ȭ �ʿ�
			pos = strstr(readData, "rssi=") + 5;
			pos2 = strrchr(readData, ',');
			strncpy(rssi, pos, pos2 - pos);
			temp_data->rssi = atof(rssi);

			//�ð�
			pos = strstr(readData, "T") + 1;
			strncpy(time, pos, 12);
			time[12] = '\0';

			//�ð�
			pos = strtok(time, ":");
			temp_data->identifiedTime += atof(pos) * 3600 * 1000;
			//��
			pos = strtok(NULL, ":");
			temp_data->identifiedTime += atof(pos) * 60 * 1000;
			//��+�и���
			pos = strtok(NULL, ":");
			temp_data->identifiedTime += atof(pos) * 1000;

			if (strcmp(targetID, temp_data->id) == 0) {
				insertList(list, temp_data, 0);
			}
			else {
				i = 0;
				while ((strcmp(referenceIDs[i++], temp_data->id) != 0)) {}
				insertList(list, temp_data, i);
			}
			strcpy(rssi, "    ");
		}
	}
	fclose(RFID_DATA);
}

//Reference Tag �м� �� ������ ��� ���
//�� Reference Tag�� RSSI ��հ��� �ð� ��հ�, Tag_ID�� �����
//�׿� ���ÿ� SortedList�� ������ Reference Tag�� �����͸� ������
//���������� �����Ͱ� 1���̶� ���� Reference Tag�� ������ ��ȯ��
int referTagAnalysis(LIST *list[], S_LIST **sorted, int flag) {

	TAGINFO *temp;
	double avg_rssi = 0, avg_time = 0;
	int count = 0, rf_count = 0;
	int i;

	for (i = 1; i <= 60; i++) {
		printf("===================================\n");
		printf("Tag_ID : %s\n", referenceIDs[i - 1]);
		if (list[i] != NULL) {
			temp = list[i]->first;
			avg_rssi = 0; avg_time = 0;
			count = 0;
			while (temp != NULL) {
				avg_rssi += temp->rssi;
				avg_time += temp->identifiedTime - list[0]->first->identifiedTime;
				temp = temp->next;
				count++;
			}
			avg_rssi = avg_rssi / (double)count;
			avg_time = avg_time / (double)count;
			printf("Reference Tag\nRssi ��� : %.3f\nTime ��� : %.3f\n", avg_rssi, avg_time);
			if(flag == 0) *sorted = insertSortList(*sorted, avg_rssi, i);
			rf_count++;
		}
		else {
			printf("�ش� �±� �����Ͱ� �����ϴ�.\n");
		}
		printf("===================================\n");
		printf("\n");
	}
	return rf_count;
}

//Target Tag �м� �� ������ ��� ���
//Target Tag�� RSSI ��հ��� �ð� ��հ�, Tag_ID�� �����
//���������� Target Tag�� ��� RSSI���� ��ȯ��
double targetTagAnalysis(LIST *list[]) {
	TAGINFO *temp = list[0]->first;
	int count = 0;
	double avg_rssi = 0, avg_time = 0;
	if (temp != NULL) {
		while (temp != NULL) {
			avg_rssi += temp->rssi;
			avg_time += temp->identifiedTime - list[0]->first->identifiedTime;
			temp = temp->next;
			count++;
		}
		avg_rssi = avg_rssi / (double)count;
		avg_time = avg_time / (double)count;
		printf("===================================\n");
		printf("Target Tag\nTag_ID : %s\nRssi ��� : %.3f\nTIme ��� : %.3f\n",list[0]->first->id, avg_rssi, avg_time);
		printf("===================================\n");
		printf("\n");
	}
	return avg_rssi;
}

//Referenence Tag�� RSSI���� �������� Target Tag�� ��ġ�� �߷��ϴ� ���
//SortedList�� �̸� �о�� �����͵��� �������� Target Tag�� ��ġ ������ ������
//kNN�˰����� �̿��� ����ġ�� ������ ��ǥ�� �����
void defineTagetPos(S_LIST *list,double avg_rssi, int rf_count) {
	int k , i;
	double sum_data1 = 0.0, sum_data2 = 0.0;
	S_LIST head = *list;
	SORTEDLIST *temp = head.first;
	SORTEDLIST *search_set;
	double *w, w_sum = 0.0;
	double x = 0.0, y = 0.0;
	printf("===================================\n");
	while (1) {
		printf("����� ����ϴ� k���� �Է����ּ��� : ");
		scanf("%d", &k);

		if (k > 0 && k < rf_count) {
			break;
		}
		else {
			printf("->�Է°��� k�� ���� �ȿ� �����ϴ�.\n");
			while (getchar() != '\n');
		}
	}
	search_set = (SORTEDLIST*)malloc(sizeof(SORTEDLIST)*k);
	w = (double*)malloc(sizeof(double)*k);

	temp = head.first;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	do {
		sum_data1 = 0.0;
		sum_data2 = 0.0;
		temp = head.first;
		for (i = 0; i < k; i++) {
			search_set[i] = *temp;
			temp = temp->next;
			sum_data1 += fabs(search_set[i].rssi - avg_rssi);
		}
		head.first = head.first->next;
		temp = head.first;
		for (i = 0; i < k; i++) {
			search_set[i] = *temp;
			temp = temp->next;
			sum_data2 += fabs(search_set[i].rssi - avg_rssi);
		}
	} while (sum_data1 >= sum_data2);
	
	temp = head.first;
	temp = temp->previous;
	for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
	}
	//kNN�˰��� ���ۺ�
	for (i = 0; i < k; i++) {
		w[i] = 1 / pow(fabs(avg_rssi - search_set[i].rssi), 2);
		w_sum += w[i];
	}
	for (i = 0; i < k; i++) {
		x += (w[i] / w_sum) * (double)search_set[i].pos[0];
		y += (w[i] / w_sum) * (double)search_set[i].pos[1];
	}
	printf("Target Tag�� ��ġ : (%.3f, %.3f)\n\n",x,y);
	printInfo(search_set, k, (int)x, (int)y);

	free(search_set);
}
//�±� ��ġ���� ���
//Target Tag �� kNN �˰��򿡼� ���� k���� Reference Tag�� ��ġ������ �����
void printInfo(SORTEDLIST list[], int size, int t_x, int t_y) {
	char mark[11][3] = { {"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"} };
	int field[23][13] = { 0 };
	int i,j;
	
	printf("Target Tag : ��\n");
	for (i = 0; i < size; i++) {
		field[list[i].pos[0] / 8][list[i].pos[1] / 12] = (i % 10) + 1;
		printf("%s : (%3d,%3d) RSSI : %.3f\n", mark[field[list[i].pos[0] / 8][list[i].pos[1] / 12] - 1], list[i].pos[0], list[i].pos[1], list[i].rssi);
	}
	field[t_x / 8][t_y / 12] = 11;
	printf("===============================================\n");
	for (i = 0; i < 13; i++) {
		for (j = 0; j < 23; j++) {
			if (field[j][i] != 0) {
				if (field[j][i] != 11) {
					printf("%s", mark[field[j][i] - 1]);
				}
				else {
					printf("��");
				}
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}
	printf("===============================================\n");
}