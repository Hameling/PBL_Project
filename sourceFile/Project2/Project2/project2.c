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

typedef struct List {
	 TAGINFO *first, *end;
}LIST;

//RSSI �� ������ ���Ľ�Ű�� ���� ����ü
typedef struct sortedList {
	char id[27];
	int pos[2];
	float rssi;
	//struct sortedList *next;
	struct sortedList *previous, *next;
}SORTEDLIST;

typedef struct S_List {
	SORTEDLIST *first, *end;
}S_LIST;


void initList(TAGINFO *list);									//���Ḯ��Ʈ �ʱ�ȭ
void insertList(LIST **list, TAGINFO *input, int p);			//���Ḯ��Ʈ�� ����ü �Է¹޾Ƽ� ����
S_LIST* insertSortList(S_LIST *list, TAGINFO *input, int p);				//
S_LIST* readFile(LIST *list[], S_LIST *sorted);					//������ �о���� �Լ� (tagList, referList, sortList) �Լ� ���ο��� insertList ȣ��
int referTagAnalysis(LIST *list[]);							//���۷��� �±� �м� ���
double targetTagAnalysis(LIST *list[]);							//Ÿ�� �±� �м� ���
void defineTagetPos(S_LIST *list, double avg_rssi, int rf_count);								//Ÿ�� ��ġ �߷�
void printInfo();												//�±׳��� ���
void printS(S_LIST *list);//test��

int main() {
	char input;
	LIST *data_set[61] = { NULL };
	S_LIST *s_list = NULL;
	double avg_rssi;
	int rf_count;

	s_list = readFile(data_set, s_list);
	avg_rssi = targetTagAnalysis(data_set);
	rf_count = referTagAnalysis(data_set);
	system("cls");
	
	//printS(s_list);

	printf("=============================================================\n");
	printf("   RFID Tag Information Analysis based Localization System\n");
	printf("=============================================================\n");
	printf("  A. Reference Tag Analysis\n");
	printf("  B. Target Tag Analysis\n");
	printf("  C. Estimation of Target Position\n");
	printf("  D. Quit\n");

	while (1) {
		printf(">>");
		scanf("%c", &input);

		if (input != 'A' && input != 'B' && input != 'C' && input != 'D') {
			printf("Unknown Input\n");
			printf("Please Select the A to D\n");

		}else if (input == 'A') {
			printf("A. Reference Tag Analysis\n");
			referTagAnalysis(data_set);
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

	}
	printf("���α׷��� ���� ����Ǿ����ϴ�.\n");
	return 0;
}

void initList(TAGINFO *list) {
	strcpy(list->id, "");
	list->rssi = 0.0;
	list->identifiedTime = 0.0;
	list->next = NULL;
}

void insertList(LIST **list, TAGINFO *input, int p) {
	//printf("%p\n", input);
	if (list[p]== NULL) {
		list[p] = (LIST*)malloc(sizeof(LIST));
		list[p]->first = input;
		list[p]->end = list[p]->first;
	}
	else {
		list[p]->end->next = input;
		list[p]->end = input;
	}
	//printf("%p\n", list[p]);
}

S_LIST* insertSortList(S_LIST *list, TAGINFO *input, int p) {
	SORTEDLIST *head;
	int flag = 0;

	SORTEDLIST *temp = (SORTEDLIST*)malloc(sizeof(SORTEDLIST));
	strcpy(temp->id, input->id);
	temp->rssi = input->rssi;
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

S_LIST* readFile(LIST *list[], S_LIST *sorted) {

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
			
			//printf("%s\n", readData);

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

			//printf("%s\n%f\n%f\n", temp_data->id, temp_data->rssi, temp_data->identifiedTime);

			if (strcmp(targetID, temp_data->id) == 0) {
				//printf("Target Tag�Դϴ�\n");
				insertList(list, temp_data, 0);
			}
			else {
				i = 0;
				while ((strcmp(referenceIDs[i++], temp_data->id) != 0)) {}
				//printf("%d��° Reference Tag�Դϴ�\n", i);
				insertList(list, temp_data, i);
				sorted = insertSortList(sorted, temp_data, i);
			}
			//printf("%p\n", list[i]);
			//printf("%s\n%f\n%f\n", list[i]->id, list[i]->rssi, list[i]->identifiedTime);
			strcpy(rssi, "    ");
			//�׽�Ʈ��
			//printf("%d��° ����\n", count++);
		}
		
	}
	
	//printS(sorted);
	fclose(RFID_DATA);
	return sorted;
}

int referTagAnalysis(LIST *list[]) {
	TAGINFO *temp = list[19]->first;
	double avg_rssi = 0, avg_time = 0;
	int count = 0;
	int f_count = 0;
	int i;

	for (i = 1; i <= 60; i++) {
		printf("Tag_ID : %s\n", referenceIDs[i - 1]);
		if (list[i] != NULL) {
			temp = list[i]->first;
			//printf("Tag_ID : %s\n", temp->id);
			avg_rssi = 0; avg_time = 0;
			count = 0;
			while (temp != NULL) {
				//printf("%s\n%f\n%f\n", temp->id, temp->rssi, temp->identifiedTime);
				avg_rssi += temp->rssi;
				avg_time += temp->identifiedTime - list[0]->first->identifiedTime;
				temp = temp->next;
				count++;
			}
			avg_rssi = avg_rssi / (double)count;
			avg_time = avg_time / (double)count;
			printf("Reference Tag\nRssi ��� : %.3f\nTIme ��� : %.3f\n", avg_rssi, avg_time);
			f_count += count;
		}
		else {
			printf("�ش� �±� �����Ͱ� �����ϴ�.\n");
		}
		printf("\n");
	}
	return f_count;
}

double targetTagAnalysis(LIST *list[]) {
	TAGINFO *temp = list[0]->first;
	int count = 0;
	double avg_rssi = 0, avg_time = 0;
	if (temp != NULL) {
		while (temp != NULL) {
			//printf("%s\n%f\n%f\n", temp->id, temp->rssi, temp->identifiedTime);
			avg_rssi += temp->rssi;
			avg_time += temp->identifiedTime - list[0]->first->identifiedTime;
			temp = temp->next;
			count++;
		}
		avg_rssi = avg_rssi / (double)count;
		avg_time = avg_time / (double)count;
		printf("Target Tag\nTag_ID : %s\nRssi ��� : %.3f\nTIme ��� : %.3f\n",list[0]->first->id, avg_rssi, avg_time);
	}
	return avg_rssi;
}

void defineTagetPos(S_LIST *list,double avg_rssi, int rf_count) {
	int k , i;
	double sum_data1 = 1.0, sum_data2 = 0.0;
	S_LIST *head = list;
	SORTEDLIST *temp = head->first;
	SORTEDLIST *search_set;
	//printS(list);
	while (1) {
		printf("�˻��� ����ϴ� k���� �Է����ּ��� : ");
		scanf("%d", &k);
		
		if (k > 0 && k < rf_count) {
			break;
		}
		else {
			printf("->�Է°��� ������ �ʰ��Ͽ����ϴ�.\n");
			while (getchar() != '\n');
		}
	}
	search_set = (SORTEDLIST*)malloc(sizeof(SORTEDLIST)*k);
	
	while (sum_data1 >= sum_data2) {
		sum_data1 = 0.0;
		sum_data2 = 0.0;
		temp = head->first;
		for (i = 0; i < k; i++) {
			search_set[i] = *temp;
			temp = temp->next;
			printf("%.3f\n", search_set[i].rssi);
			sum_data1 += fabs(search_set[i].rssi - avg_rssi);
		}
		printf("A�� : %.3f\n", sum_data1);
		
		head->first = head->first->next;
		temp = head->first;
		//temp = temp->next;
		for (i = 0; i < k; i++) {
			search_set[i] = *temp;
			temp = temp->next;
			printf("%.3f\n", search_set[i].rssi);
			sum_data2 += fabs(search_set[i].rssi - avg_rssi);
		}
		printf("B�� : %.3f\n", sum_data2);

		
		//head->first = head->first->next;
	}
	free(search_set);

	/*for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
		printf("%.3f\n", search_set[i].rssi);
		sum_data1 += fabs(search_set[i].rssi - avg_rssi);
	}
	printf("�� : %.3f\n", sum_data1);

	temp = head->first;
	temp = temp->next;
	for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
		printf("%.3f\n", search_set[i].rssi);
		sum_data2 += fabs(search_set[i].rssi - avg_rssi);
	}
	printf("�� : %.3f\n", sum_data2);

	sum_data1 = 0.0;
	sum_data2 = 0.0;
	head->first = head->first->next;

	temp = head->first;
	for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
		printf("%.3f\n", search_set[i].rssi);
		sum_data1 += fabs(search_set[i].rssi - avg_rssi);
	}
	printf("�� : %.3f\n", sum_data1);

	temp = head->first;
	temp = temp->next;
	for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
		printf("%.3f\n", search_set[i].rssi);
		sum_data2 += fabs(search_set[i].rssi - avg_rssi);
	}
	printf("�� : %.3f\n", sum_data2);*/
}

void printS(S_LIST *list) {
	SORTEDLIST *temp = list->first;
	if (temp != NULL) {
		while (temp != NULL) {
			printf("Target Tag\nTag_ID : %s\nRssi : %.3f\n\n", temp->id, temp->rssi);
			temp = temp->next;
		}
	}
}