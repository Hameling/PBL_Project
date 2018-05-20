#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<Windows.h>
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

void initList(TAGINFO *list);											//TagInfo 연결리스트 초기화
void insertList(LIST **list, TAGINFO *input, int p);					//TagInfo 연결리스트에 정보를 저장하기 위한 함수
S_LIST* insertSortList(S_LIST *list, float rssi, int p);				//SortedList 연결리스트에 정보를 저장하기 위한 함수
void initMenu();														//메뉴 선택화면 출력을 위한 함수 
void readFile(LIST *list[]);											//파일을 읽어오는 함수 
int referTagAnalysis(LIST *list[], S_LIST **sorted, int flag);					//Reference Tag 분석 및 데이터 출력 기능
double targetTagAnalysis(LIST *list[]);									//Target Tag 분석 및 데이터 출력 기능
void defineTagetPos(S_LIST *list, double avg_rssi, int rf_count);						//Referenence Tag의 RSSI값을 바탕으로 Target Tag의 위치를 추론하는 기능
void printInfo(SORTEDLIST list[], int size, int t_x, int t_y);												//태그내용 출력

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

void initList(TAGINFO *list) {
	strcpy(list->id, "");
	list->rssi = 0.0;
	list->identifiedTime = 0.0;
	list->next = NULL;
}

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

int referTagAnalysis(LIST *list[], S_LIST **sorted, int flag) {

	TAGINFO *temp;
	double avg_rssi = 0, avg_time = 0;
	int count = 0, rf_count = 0;
	int i;

	for (i = 1; i <= 60; i++) {
		printf("===================================================\n");
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

		printf("===================================================\n");
		printf("\n");

	}
	return rf_count;
}

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

		printf("===================================================\n");
		printf("Target Tag\nTag_ID : %s\nRssi 평균 : %.3f\nTIme 평균 : %.3f\n",list[0]->first->id, avg_rssi, avg_time);
		printf("===================================================\n");
		printf("\n");
	}
	return avg_rssi;
}

void defineTagetPos(S_LIST *list,double avg_rssi, int rf_count) {
	int k , i;
	double sum_data1 = 0.0, sum_data2 = 0.0;
	S_LIST head = *list;
	SORTEDLIST *temp = head.first;
	SORTEDLIST *search_set;
	double *w, w_sum = 0.0;
	double x = 0.0, y = 0.0;
	
	printf("===================================================\n");
	while (1) {
		printf("검색을 희망하는 k값을 입력해주세요 : ");
		scanf("%d", &k);

		if (k > 0 && k < rf_count) {
			break;
		}
		else {
			printf("->입력값이 범위를 초과하였습니다.\n");
			while (getchar() != '\n');
		}
	}
	search_set = (SORTEDLIST*)malloc(sizeof(SORTEDLIST)*k);
	w = (double*)malloc(sizeof(double)*k);

	temp = head.first;
	while (temp->next != NULL) {
		//printf("RSSI 차이값 : %.3f\n", fabs(temp->rssi - avg_rssi));
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
	
	//테스트
	temp = head.first;
	temp = temp->previous;
	for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
		//printf("Tag_ID : %s\nRSSI차이 : %.3f\nPos :  (%d,%d)\n", search_set[i].id,fabs(avg_rssi - search_set[i].rssi), search_set[i].pos[0],search_set[i].pos[1]);
	}
	

	for (i = 0; i < k; i++) {
		w[i] = 1 / pow(fabs(avg_rssi - search_set[i].rssi), 2);
		w_sum += w[i];
	}

	/*for (i = 0; i < k; i++) {
		printf("%d번쨰 실가중치 : %.3f\n", i + 1, (w[i] /  w_sum));
	}*/

	for (i = 0; i < k; i++) {
		x += (w[i] / w_sum) * (double)search_set[i].pos[0];
		y += (w[i] / w_sum) * (double)search_set[i].pos[1];
	}
	printf("X : %.3f\nY : %.3f\n", x, y);
	
	printInfo(search_set, k, (int)x, (int)y);

	free(search_set);
}

void printInfo(SORTEDLIST list[], int size, int t_x, int t_y) {
	HWND hwnd;
	HDC hdc;
	int i;
	char temp_pos[20] = "";
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd);
	
	system("cls");
	printf("=============================================================\n");
	for (i = 0; i < 15; i++) {
		printf("\n");
	}
	printf("=============================================================\n");
	
	for (i = 0; i < size; i++) {
		sprintf(temp_pos, "(%d,%d)", list[i].pos[0], list[i].pos[1]);
		printf("%s\n", temp_pos);

		//TextOutA(hdc, list[i].pos[0], list[i].pos[1], temp_pos, strlen(temp_pos));
		//SetPixel(hdc, list[i].pos[0], list[i].pos[1], RGB(255, 255, 255));
		Ellipse(hdc,list[i].pos[0]+17, list[i].pos[1]+20, list[i].pos[0]+27, list[i].pos[1]+30);
		strcpy(temp_pos, "");
	}
	sprintf(temp_pos, "(%d,%d)", t_x, t_y);
	//TextOutA(hdc, t_x, t_y, temp_pos, strlen(temp_pos));
	//SetPixel(hdc, t_x, t_y, RGB(255, 255, 255));
	Rectangle(hdc, t_x + 17, t_y + 20, t_x + 27, t_y + 30);


}