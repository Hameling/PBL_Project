#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"tag_ID_position.h"
#pragma warning(disable:4996)

//RFID 태그 정보를 저장하기 위한 구조체
typedef struct taginfo {
	char id[27];
	float rssi;
	double identifiedTime;
	struct taginfo *next;
}TAGINFO;

//RFID 태그 정보의 활용을 위해 연결리스트의 처음과 끝을 저장하기 위한 구조체
typedef struct List {
	 TAGINFO *first, *end;
}LIST;

//RSSI 값 순으로 정렬시키기 위한 구조체
typedef struct sortedList {
	char id[27];
	int pos[2];
	float rssi;
	struct sortedList *previous, *next;
}SORTEDLIST;

//정렬된 구조체의 활용을 위해 연결리스트의 처음과 끝을 저장하는 구조체
typedef struct S_List {
	SORTEDLIST *first, *end;
}S_LIST;

void initList(TAGINFO *list);											//TagInfo 연결리스트 초기화를 위한 함수
void insertList(LIST **list, TAGINFO *input, int p);					//TagInfo 연결리스트에 정보를 저장하기 위한 함수
S_LIST* insertSortList(S_LIST *list, float rssi, int p);				//SortedList 연결리스트에 정보를 오름차순으로 저장하기 위한 함수

void initMenu();														//메뉴 선택화면을 출력하기 위한 함수 
void readFile(LIST *list[]);											//파일을 읽어와 각 태그의 정보값을 연결리스트에 저장하는 함수

int referTagAnalysis(LIST *list[], S_LIST **sorted, int flag);			//Reference Tag 분석 및 데이터 출력 기능
double targetTagAnalysis(LIST *list[]);									//Target Tag 분석 및 데이터 출력 기능
void defineTagetPos(S_LIST *list, double avg_rssi, int rf_count);		//Referenence Tag의 RSSI값을 바탕으로 Target Tag의 위치를 추론하는 기능
void printInfo(SORTEDLIST list[], int size, int t_x, int t_y);			//태그 위치정보 출력

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
	printf("프로그램이 정상 종료되었습니다.\n");
	return 0;
}

//TagInfo 연결리스트 초기화를 위한 함수
void initList(TAGINFO *list) {
	strcpy(list->id, "");
	list->rssi = 0.0;
	list->identifiedTime = 0.0;
	list->next = NULL;
}

//TagInfo 연결리스트에 정보를 저장하기 위한 함수
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

//SortedList 연결리스트에 정보를 오름차순으로 저장하기 위한 함수
//이중연결리스트로 구성된 SortedList를 초기화 및 정렬된 상태로 저장
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

//메뉴 선택화면을 출력하기 위한 함수
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

//파일을 읽어와 각 태그의 정보값을 연결리스트에 저장하는 함수
//파일을 데이터를 한줄 씩 읽어와 연결리스트에 필요한 정보를 저장
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

			//rssi 값 매번 초기화 필요
			pos = strstr(readData, "rssi=") + 5;
			pos2 = strrchr(readData, ',');
			strncpy(rssi, pos, pos2 - pos);
			temp_data->rssi = atof(rssi);

			//시간
			pos = strstr(readData, "T") + 1;
			strncpy(time, pos, 12);
			time[12] = '\0';

			//시간
			pos = strtok(time, ":");
			temp_data->identifiedTime += atof(pos) * 3600 * 1000;
			//분
			pos = strtok(NULL, ":");
			temp_data->identifiedTime += atof(pos) * 60 * 1000;
			//초+밀리초
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

//Reference Tag 분석 및 데이터 출력 기능
//각 Reference Tag의 RSSI 평균값과 시간 평균값, Tag_ID를 출력함
//그와 동시에 SortedList에 읽혀온 Reference Tag의 데이터를 저장함
//최종적으로 데이터가 1번이라도 읽힌 Reference Tag의 갯수를 반환함
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
			printf("Reference Tag\nRssi 평균 : %.3f\nTime 평균 : %.3f\n", avg_rssi, avg_time);
			if(flag == 0) *sorted = insertSortList(*sorted, avg_rssi, i);
			rf_count++;
		}
		else {
			printf("해당 태그 데이터가 없습니다.\n");
		}
		printf("===================================\n");
		printf("\n");
	}
	return rf_count;
}

//Target Tag 분석 및 데이터 출력 기능
//Target Tag의 RSSI 평균값과 시간 평균값, Tag_ID를 출력함
//최종적으로 Target Tag의 평균 RSSI값을 반환함
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
		printf("Target Tag\nTag_ID : %s\nRssi 평균 : %.3f\nTIme 평균 : %.3f\n",list[0]->first->id, avg_rssi, avg_time);
		printf("===================================\n");
		printf("\n");
	}
	return avg_rssi;
}

//Referenence Tag의 RSSI값을 바탕으로 Target Tag의 위치를 추론하는 기능
//SortedList와 미리 읽어둔 데이터들을 바탕으로 Target Tag의 위치 정보를 예측함
//kNN알고리즘을 이용해 가중치를 감안한 좌표를 계산함
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
		printf("계산을 희망하는 k값을 입력해주세요 : ");
		scanf("%d", &k);

		if (k > 0 && k < rf_count) {
			break;
		}
		else {
			printf("->입력값이 k의 범위 안에 없습니다.\n");
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
	//kNN알고리즘 시작부
	for (i = 0; i < k; i++) {
		w[i] = 1 / pow(fabs(avg_rssi - search_set[i].rssi), 2);
		w_sum += w[i];
	}
	for (i = 0; i < k; i++) {
		x += (w[i] / w_sum) * (double)search_set[i].pos[0];
		y += (w[i] / w_sum) * (double)search_set[i].pos[1];
	}
	printf("Target Tag의 위치 : (%.3f, %.3f)\n\n",x,y);
	printInfo(search_set, k, (int)x, (int)y);

	free(search_set);
}
//태그 위치정보 출력
//Target Tag 및 kNN 알고리즘에서 사용된 k개의 Reference Tag의 위치정보를 출력함
void printInfo(SORTEDLIST list[], int size, int t_x, int t_y) {
	char mark[11][3] = { {"○"},{"●"},{"◇"},{"◆"},{"□"},{"■"},{"♤"},{"♧"},{"♡"},{"◈"} };
	int field[23][13] = { 0 };
	int i,j;
	
	printf("Target Tag : ☆\n");
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
					printf("☆");
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