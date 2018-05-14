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

typedef struct List {
	 TAGINFO *first, *end;
}LIST;

//RSSI 값 순으로 정렬시키기 위한 구조체
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


void initList(TAGINFO *list);									//연결리스트 초기화
void insertList(LIST **list, TAGINFO *input, int p);			//연결리스트와 구조체 입력받아서 저장
S_LIST* insertSortList(S_LIST *list, TAGINFO *input, int p);				//
S_LIST* readFile(LIST *list[], S_LIST *sorted);					//파일을 읽어오는 함수 (tagList, referList, sortList) 함수 내부에서 insertList 호출
int referTagAnalysis(LIST *list[]);							//레퍼런스 태그 분석 기능
double targetTagAnalysis(LIST *list[]);							//타겟 태그 분석 기능
void defineTagetPos(S_LIST *list, double avg_rssi, int rf_count);								//타겟 위치 추론
void printInfo();												//태그내용 출력
void printS(S_LIST *list);//test용

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

			//printf("%s\n%f\n%f\n", temp_data->id, temp_data->rssi, temp_data->identifiedTime);

			if (strcmp(targetID, temp_data->id) == 0) {
				//printf("Target Tag입니다\n");
				insertList(list, temp_data, 0);
			}
			else {
				i = 0;
				while ((strcmp(referenceIDs[i++], temp_data->id) != 0)) {}
				//printf("%d번째 Reference Tag입니다\n", i);
				insertList(list, temp_data, i);
				sorted = insertSortList(sorted, temp_data, i);
			}
			//printf("%p\n", list[i]);
			//printf("%s\n%f\n%f\n", list[i]->id, list[i]->rssi, list[i]->identifiedTime);
			strcpy(rssi, "    ");
			//테스트용
			//printf("%d번째 실행\n", count++);
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
			printf("Reference Tag\nRssi 평균 : %.3f\nTIme 평균 : %.3f\n", avg_rssi, avg_time);
			f_count += count;
		}
		else {
			printf("해당 태그 데이터가 없습니다.\n");
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
		printf("Target Tag\nTag_ID : %s\nRssi 평균 : %.3f\nTIme 평균 : %.3f\n",list[0]->first->id, avg_rssi, avg_time);
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
		printf("A합 : %.3f\n", sum_data1);
		
		head->first = head->first->next;
		temp = head->first;
		//temp = temp->next;
		for (i = 0; i < k; i++) {
			search_set[i] = *temp;
			temp = temp->next;
			printf("%.3f\n", search_set[i].rssi);
			sum_data2 += fabs(search_set[i].rssi - avg_rssi);
		}
		printf("B합 : %.3f\n", sum_data2);

		
		//head->first = head->first->next;
	}
	free(search_set);

	/*for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
		printf("%.3f\n", search_set[i].rssi);
		sum_data1 += fabs(search_set[i].rssi - avg_rssi);
	}
	printf("합 : %.3f\n", sum_data1);

	temp = head->first;
	temp = temp->next;
	for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
		printf("%.3f\n", search_set[i].rssi);
		sum_data2 += fabs(search_set[i].rssi - avg_rssi);
	}
	printf("합 : %.3f\n", sum_data2);

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
	printf("합 : %.3f\n", sum_data1);

	temp = head->first;
	temp = temp->next;
	for (i = 0; i < k; i++) {
		search_set[i] = *temp;
		temp = temp->next;
		printf("%.3f\n", search_set[i].rssi);
		sum_data2 += fabs(search_set[i].rssi - avg_rssi);
	}
	printf("합 : %.3f\n", sum_data2);*/
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